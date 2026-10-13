# UART: LED valdymas iš kompiuterio

Atskiras CCS projektas **UART**, skirtas MSP-EXP430G2 **Rev 1.5** su MSP430G2553.
Kompiuteris siunčia tekstines komandas, mikrovaldiklis valdo P1.0 LED ir atsako.
USB kabelis jungiamas prie plokštės integruoto USB–UART tilto.

## Jungtys ir nustatymai

| Parametras | Reikšmė |
| --- | --- |
| UART | USCI_A0 |
| Mikrovaldiklio RX | P1.1 |
| Mikrovaldiklio TX | P1.2 |
| Dažnis | Kalibruotas 1 MHz DCO / SMCLK |
| Terminalas | 9600 baud, 8 bitai, be parity, 1 stop bit (8N1) |
| Flow control | None |
| Prievadas | COM4 šiame kompiuteryje; patikrink Device Manager |
| Eilutės pabaiga | CR, LF arba CRLF |

**J3 RXD/TXD trumpikliai turi būti nustatyti aparatinei UART.** Rev. 1.5
plokštėje abu UART trumpikliai pasukami 90° nuo programinės UART padėties.
Senesnėse revizijose reikia sukryžiuoti UART jungtis laidais. Tikrink savo
plokštės reviziją ir vadovo schemą; prieš perkeldamas trumpiklius atjunk USB.
VCC, TEST ir RST trumpiklių nekeisk.

Šioje Rev 1.5 plokštėje ryšys pradėjo veikti nustačius **HW UART** padėtį.
Vien tai, kad veikia programos įrašymas per USB, dar nereiškia, kad UART
trumpikliai nustatyti teisingai.

Šaltiniai: [LaunchPad vadovas](https://www.ti.com/lit/ug/slau318g/slau318g.pdf),
[Energia UART jungčių schemos](https://github.com/energia/Energia/wiki/Serial-Communication),
[MSP430G2553 dokumentacija](https://www.ti.com/product/MSP430G2553).

## Paleidimas vietiniame CCS (patikrintas būdas)

1. Atidaryk kompiuteryje įdiegtą [Code Composer Studio](https://www.ti.com/tool/CCSTUDIO).
2. Importuok šį `uart/` aplanką kaip CCS projektą ir pasirink **UART**.
3. Turi būti įdiegtas MSP430 įrenginių palaikymas bei projekto kompiliatorius.
   Pasirink mikrovaldiklį **MSP430G2553** ir jungtį **TI MSP430 USB1**.
4. Atlik **Build → Debug → Resume / Continue**. Debug lange prie MSP430
   turi būti **RUNNING**; sustojus ties `main()` programa komandų dar nepriima.
5. UART komandas siųsk per žemiau aprašytą PowerShell, atidaręs **COM4**.
   **Debug Console** nėra UART terminalas: įvedus `on` ten rodoma
   `identifier not found`, nes derintuvas bando vertinti programos išraišką.

## Alternatyva: CCS Cloud

1. Atidaryk [CCS Cloud](https://dev.ti.com/ide/).
2. Importuok šį CCS projektą, jei IDE siūlo vietinio projekto importą.
   Alternatyva: sukurk atskirą MSP430G2553 projektą `UART`, naudodamas jau
   veikusį TI pavyzdį kaip pagrindą, ir pakeisk jo C failo turinį šiuo `main.c`.
   Projekte turi likti tik vienas C failas su `main()`.
3. Pasirink MSP430G2553 ir TI MSP430 USB1, atlik **Build → Debug → Resume**.
4. Atidaryk nuosekliojo ryšio terminalą su aukščiau nurodytais nustatymais.
   Jei terminalas turi local echo nustatymą, įjunk jį, kad matytum rašomą tekstą.
5. Siųsk `help` ir Enter. Pradinį pasisveikinimą gali praleisti, jei terminalą
   atidarai po programos paleidimo; `help` veiks ir tokiu atveju.

| Komanda | Veiksmas / atsakymas |
| --- | --- |
| `on` | Įjungia LED; `LED ON` |
| `off` | Išjungia LED; `LED OFF` |
| `toggle` | Perjungia LED ir praneša būseną |
| `status` | Grąžina LED būseną |
| `help` | Parodo komandas |

Komandos rašomos mažosiomis raidėmis be papildomų tarpų. Siųsk po vieną
eilutę ir palauk atsakymo. Programa naudoja paprastą apklausą, todėl neskirta
nepertraukiamam duomenų srautui. Priima iki 23 simbolių; per ilga ar sugadinta
eilutė atmetama. CRLF ir tuščios eilutės papildomų komandų nesukelia.

## Bandymas per PowerShell

### Interaktyvus valdymas

Jei naudoji CMD arba Git Bash, pirmiausia vykdyk `powershell.exe -NoProfile`.
Turi matytis eilutės pradžia `PS C:\...>`. Komandas vykdyk tame pačiame lange.
Uždaryk kitus COM4 terminalus. Prievadą atidaryk vieną kartą:

```powershell
$uart = [System.IO.Ports.SerialPort]::new('COM4',9600,'None',8,'One'); $uart.NewLine = "`n"; $uart.ReadTimeout = 2000; $uart.WriteTimeout = 2000; $uart.Handshake = [System.IO.Ports.Handshake]::None; $uart.Open()
```

Toliau vykdyk po vieną eilutę ir palauk atsakymo:

```powershell
$uart.WriteLine('on'); $uart.ReadLine()
$uart.WriteLine('off'); $uart.ReadLine()
$uart.WriteLine('toggle'); $uart.ReadLine()
$uart.WriteLine('status'); $uart.ReadLine()
$uart.WriteLine('help'); $uart.ReadLine()
```

`on` grąžina `LED ON`, `off` – `LED OFF`. `toggle` perjungia LED,
`status` parodo jo būseną, `help` išvardija komandas.
Baigęs arba prieš atjungdamas USB atlaisvink prievadą:

```powershell
$uart.Dispose()
```

Po USB atjungimo/prijungimo sukurk naują `$uart` objektą ir atidaryk prievadą
iš naujo. COM numerį prireikus patikrink Windows Device Manager.

### Automatinė komandų seka

Jei prieš tai naudojai interaktyvų `$uart`, pirmiausia vykdyk `$uart.Dispose()`.

Uždaryk kitą COM4 naudojantį terminalą. Kai mikrovaldiklio programa paleista,
Windows PowerShell galima vykdyti:

```powershell
$uartPort = [System.IO.Ports.SerialPort]::new('COM4', 9600, 'None', 8, 'One')
$uartPort.Handshake = [System.IO.Ports.Handshake]::None
$uartPort.NewLine = "`n"
$uartPort.ReadTimeout = 2000
$uartPort.WriteTimeout = 2000
try {
    $uartPort.Open()
    Start-Sleep -Milliseconds 200
    $uartPort.DiscardInBuffer()
    foreach ($uartCommand in @('help', 'on', 'status', 'off')) {
        $uartPort.WriteLine($uartCommand)
        $uartPort.ReadLine()
    }
} finally {
    if ($uartPort.IsOpen) { $uartPort.Close() }
    $uartPort.Dispose()
}
```

Jei atsakymo nėra: patikrink J3 trumpiklius, COM numerį, **Resume** būseną ir
9600 / 8N1 / None nustatymus. Jei tekstas iškraipytas, patikrink greitį ir
taktinio dažnio nustatymus. Jei kalibravimo konstantos ištrintos, programa
palieka LED įjungtą ir UART nepaleidžia.

## Derinimas ir dažnos klaidos

| Požymis | Ką tikrinti |
| --- | --- |
| `'$uart' is not recognized` | Paleisk PowerShell; CMD šios sintaksės nesupranta. |
| `identifier not found: on` | Komanda įvesta į Debug Console; naudok PowerShell ir COM4. |
| `ReadLine` timeout, LED nereaguoja | HW UART trumpikliai, COM numeris, UART projekto įrašymas ir RUNNING būsena. |
| COM prievadas užimtas / access denied | Uždaryk kitą terminalą arba ankstesnį SerialPort objektą. |
| Gavimas nutrūko po USB perjungimo | Atlaisvink seną objektą ir iš naujo atidaryk COM prievadą. |

Norėdamas sekti komandą CCS, uždėk breakpoint ties `command(line)` iškvietimu
faile `main.c`, paleisk **Resume** ir PowerShell išsiųsk tik
`$uart.WriteLine('on')`. Kai CCS sustos, apžiūrėk `line` ir vykdyk žingsniais.
Prieš vykdydamas `$uart.ReadLine()` vėl paspausk **Resume**, kad programa
galėtų išsiųsti atsakymą. Kol procesorius sustabdytas, naujų komandų nesiųsk.
Įprastam bandymui išjunk breakpoint'us ir palik **RUNNING**.

## Patikrinimas plokštėje

2026-10-13 naudotojas vietiniame CCS paleido UART projektą su MSP430G2553,
MSP-EXP430G2 Rev 1.5 plokštėje nustatė HW UART trumpiklius ir per PowerShell
COM4 / 9600 / 8N1 išsiuntė `on`. Gautas atsakymas **`LED ON`** patvirtino
ryšį abiem kryptimis. Kitų komandų rezultatai dar nebuvo atskirai patvirtinti.
