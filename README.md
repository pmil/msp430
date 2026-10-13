# MSP430 LaunchPad

Mokomasis LED mirksėjimo projektas, sukurtas ir paleistas su CCS Cloud.

## Naudojama įranga

| Parametras | Reikšmė |
| --- | --- |
| Plokštė | Texas Instruments MSP-EXP430G2 LaunchPad |
| Mikrovaldiklis | MSP430G2553 |
| LED išvadas | P1.0 |
| Programavimo jungtis CCS | TI MSP430 USB1 |
| UART prievadas šiame kompiuteryje | COM4 (kitame kompiuteryje gali skirtis) |
| IDE | Code Composer Studio Cloud (CCS Cloud) |
| Kompiliatorius eksportuotame projekte | TI MSP430 21.6.3.LTS |
| CCS projekto pavadinimas | Test |

Mikrovaldiklio dokumentacija: [MSP430G2553](https://www.ti.com/product/MSP430G2553).
Plokštės vadovas: [MSP-EXP430G2, SLAU318](https://www.ti.com/lit/ug/slau318g/slau318g.pdf).

## IDE ir pavyzdžio šaltinis

Naudojame naršyklėje veikiančią **CCS Cloud**: pilnos IDE šiame projekte
nesisiuntėme ir lokaliai nediegėme.

- [TI Developer Zone](https://dev.ti.com/) – pradinis TI įrankių puslapis.
- [CCS Cloud IDE](https://dev.ti.com/ide/) – programos redagavimas, kompiliavimas ir derinimas.
- [TI Resource Explorer](https://dev.ti.com/tirex/) – pavyzdžiai ir dokumentacija.
- [MSP430G2553 projektų vedlys](https://dev.ti.com/tirex/wizard?devices=MSP430G2553&isTheia=false) – nuoroda iš eksportuoto projekto konfigūracijos.
- [CCS darbalaukio versijos atsisiuntimas](https://www.ti.com/tool/CCSTUDIO) – alternatyva darbui lokaliai; šiame projekte nenaudota.

Pavyzdys paimtas iš TI Resource Explorer: **msp430g2xx3_1.c – Software Toggle P1.0**.
Paieškoje pasirink MSP430G2553 ir ieškok šio pavyzdžio MSP430Ware / Code Examples.
Jei siūlomas importas į CCS Cloud, importuok projektą.

CCS Cloud ryšiui su USB plokšte gali paprašyti įdiegti TI Cloud Agent;
vadovaukis IDE pateiktais diegimo nurodymais. Šiame kompiuteryje plokštė
aptikta automatiškai, Windows USB ir HID tvarkyklės rodė būseną OK.
COM4 skirtas UART ryšiui; projekto programavimo jungtis yra TI MSP430 USB1.

## Projekto failai

`blink/` aplanke saugomas iš CCS Cloud eksportuotas projektas `Test`:

- `msp430g2xx3_1.c` – LED mirksėjimo kodas su TI licencija.
- `lnk_msp430g2553.cmd` – mikrovaldiklio atminties išdėstymo linkerio failas.
- `.project`, `.cproject`, `.ccsproject`, `.settings/` – CCS projekto nustatymai.
- `targetConfigs/MSP430G2553.ccxml` – mikrovaldiklio ir programavimo jungties konfigūracija.

Eksporto archyvas `Test.tar` buvo išarchyvuotas ir pašalintas.
`.gitignore` neįtraukia eksporto archyvo, Debug/Release rezultatų ir vietinių talpyklų.

## Kompiliavimas ir paleidimas

1. Prijunk LaunchPad prie kompiuterio USB laidu.
2. CCS Cloud atidaryk projektą `Test` arba importuok aukščiau nurodytą TI pavyzdį.
   Norėdamas atkurti saugykloje esančią versiją, naudok `blink/msp430g2xx3_1.c` turinį.
3. Patikrink, kad pasirinktas MSP430G2553 ir TI MSP430 USB1.
4. Paspausk **Build** ir patikrink, ar nėra kompiliavimo klaidų.
5. Paspausk **Debug** – programa bus įrašyta į mikrovaldiklį, pakeisdama ankstesnę.
6. Jei vykdymas sustoja ties `main()`, paspausk **Resume / Continue**.
7. Plokštės P1.0 LED turi mirksėti.

## Kaip veikia programa

Programa sustabdo watchdog laikmatį, nustato P1.0 kaip išėjimą ir begaliniame
cikle perjungia LED būseną naudodama `P1OUT ^= 0x01`.
Tarp perjungimų kintamasis `i` mažinamas iki nulio.

Pradinio pavyzdžio `i = 50000` pakeista į **`i = 10000`**, todėl LED mirksi greičiau.
Tai programinė delsa: tiksli trukmė priklauso nuo taktinio dažnio ir kompiliavimo.
Šiame projekte `unsigned int` yra 16 bitų (didžiausia reikšmė 65535).

2026-10-13 naudotojas CCS Cloud sukompiliavo ir įrašė programą į plokštę;
patvirtino LED mirksėjimą ir jo pagreitėjimą pakeitus delsą.

## Darbas su GitHub

Saugykla: [pmil/msp430](https://github.com/pmil/msp430).
Vietinis aplankas: `C:\github\msp430`.

CCS Cloud pakeitimai automatiškai nesinchronizuoja su šia vietine saugykla.
Po darbo eksportuok atnaujintą projektą arba nukopijuok pakeistus šaltinius į
`blink/`, peržiūrėk skirtumus ir įkelk:

```powershell
cd C:\github\msp430
git status
git diff
git add README.md .gitignore blink
git commit -m "Aprasyti pakeitimai"
git push
```

TI pavyzdžio licencijos sąlygos išsaugotos šaltinio failuose.
