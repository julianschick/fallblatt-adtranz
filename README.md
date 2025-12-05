# Adtranz-Fallblattmodule
![Buchstabensuppe](./images/buchstabensuppe.png)

ESP32-Mikrocontrollercode und Leiterplattenlayouts für den Betrieb von 20 Adtranz-Fallblattmodulen mit Alphanumerischer Zeichenpalette.

## Funktionsweise

Wie in (diesem Mikrocontroller.net-Thread)[https://www.mikrocontroller.net/topic/460531] auch gut mit Schaltplan erläutert, sind die Fallblattmodule mit einer absoluten Positionierung anhand einer 6-Bit-Gray-Codierungsscheibe versehen. 6 Infrarotdioden sind hinter einer auf der Hauptachse mitrotierenden Scheibe angebracht, deren ringförmige Aussparungen einen Gray-Code ergeben, der mit Infrarotsensoren auf der anderen Seite der Scheibe ausgelesen werden kann.

Meine Ansteuerung besteht aus einem Zentralmodul und einem Steckermodul für jedes einzelne Fallblattmodul. Das Steckermodul wird mit einen "Card-Edge-Connector" direkt an das Fallblattmodul angesteckt. Auf dem Steckermodul befindet sich lediglich ein Shift-Register und ein Komparatorchip. Der Motor wird, solange mit Betriebsspannung versorgt, solange weiterdrehen, bis der im Shift-Register eingestellte Code auf der Codierungsscheibe erreicht ist.

Auf dem Zentralmodul sitzt ein ESP32, der die als Daisy-Chain verbundenen Steckermodule bzw. deren Shift-Register als ein langes Shift-Register befüllt. Da die Steuerleitungen dieser Shift-Registerkette durchaus lang sind (ca. 2m) ist es erforderlich diese direkt am Ausgang des Mikrocontrollers seriell zu terminieren, da die saubere Signalübertragung durch Spiegelungseffekte andernfalls nicht mehr gewährleistet ist.

## Leiterplattenlayouts

<img alt="Steckermodul" src="https://github.com/julianschick/fallblatt-adtranz/blob/main/images/plug-module.png" float="right" width="49%"/> <img alt="Zentralmodul" src="https://github.com/julianschick/fallblatt-adtranz/blob/main/images/central-module.png" float="left" width="49%"/> 

