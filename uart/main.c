#include <msp430.h>
#include <string.h>

/* MSP430G2553: P1.1 = RX, P1.2 = TX, P1.0 = LED.
 * 1 MHz SMCLK, 9600 baud, 8 data bits, no parity, 1 stop bit.
 * Send one command per line, then wait for the response.
 */
#define LINE_SIZE 24

static void uart_puts(const char *text)
{
    while (*text != '\0') {
        while (!(IFG2 & UCA0TXIFG)) {
        }
        UCA0TXBUF = *text++;
    }
}

static void report_led(void)
{
    uart_puts((P1OUT & BIT0) ? "LED ON\r\n" : "LED OFF\r\n");
}

static void command(const char *line)
{
    if (strcmp(line, "on") == 0) {
        P1OUT |= BIT0;
        report_led();
    } else if (strcmp(line, "off") == 0) {
        P1OUT &= ~BIT0;
        report_led();
    } else if (strcmp(line, "toggle") == 0) {
        P1OUT ^= BIT0;
        report_led();
    } else if (strcmp(line, "status") == 0) {
        report_led();
    } else if (strcmp(line, "help") == 0) {
        uart_puts("Commands: on, off, toggle, status, help\r\n");
    } else {
        uart_puts("ERR unknown command; type help\r\n");
    }
}

int main(void)
{
    char line[LINE_SIZE];
    unsigned int length = 0;
    unsigned char invalid = 0;
    unsigned char rx_status;
    unsigned char ch;

    WDTCTL = WDTPW | WDTHOLD;
    P1OUT &= ~BIT0;
    P1DIR |= BIT0;

    /* An erased calibration constant cannot produce reliable UART timing. */
    if (CALBC1_1MHZ == 0xFF || CALDCO_1MHZ == 0xFF) {
        P1OUT |= BIT0;
        for (;;) {
        }
    }
    DCOCTL = 0;
    BCSCTL1 = CALBC1_1MHZ;
    DCOCTL = CALDCO_1MHZ;
    BCSCTL2 = 0;                 /* SMCLK = DCO, divider = 1. */

    P1SEL |= BIT1 | BIT2;
    P1SEL2 |= BIT1 | BIT2;
    UCA0CTL1 = UCSWRST;
    UCA0CTL0 = 0;                /* Asynchronous UART, 8N1, LSB first. */
    UCA0CTL1 |= UCSSEL_2;
    UCA0BR0 = 104;
    UCA0BR1 = 0;
    UCA0MCTL = UCBRS0;
    UCA0CTL1 &= ~UCSWRST;

    uart_puts("MSP430 UART ready. Type help + Enter.\r\n");

    for (;;) {
        while (!(IFG2 & UCA0RXIFG)) {
        }
        /* Read error flags before RXBUF clears them. Reject damaged lines. */
        rx_status = UCA0STAT;
        ch = UCA0RXBUF;
        if (rx_status & (UCOE | UCFE | UCPE | UCBRK)) {
            invalid = 1;
        }

        if (ch == '\r' || ch == '\n') {
            if (invalid) {
                uart_puts("ERR invalid/too long line; resend\r\n");
            } else if (length != 0) {
                line[length] = '\0';
                command(line);
            }
            length = 0;
            invalid = 0;
        } else if (!invalid) {
            if (ch == '\b' || ch == 127) {
                if (length != 0) {
                    --length;
                }
            } else if (ch >= 32 && ch <= 126) {
                if (length < LINE_SIZE - 1) {
                    line[length++] = (char)ch;
                } else {
                    invalid = 1;
                }
            } else {
                invalid = 1;
            }
        }
    }
}
