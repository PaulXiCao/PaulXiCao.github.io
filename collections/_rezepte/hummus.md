---
title: "Hummus"
date: 2024-03-23
layout: posts
categories:
  - Rezepte
---

## Zutaten

- 450 g Kichererbsen (Abtropfgewicht)
- 40 g Tahini
- 1 TL Salz
- 2 TL [Ras el hanout](https://www.rewe.de/gewuerzlexikon/ras-el-hanout/)
- 2 TL Gewürzmischung
- 1/2 TL [Sumak](https://www.rewe.de/gewuerzlexikon/sumach/)
- 15 Drehungen Pfeffermühle
- 30 g Tomatenmark
- 1 kleine Zitrone
- Wasser bis bedeckt
- zum Verzieren: 
  - [Zatar](https://www.rewe.de/gewuerzlexikon/zatar/)
  - 25 g Olivenöl

## Hilfsmittel

- Zitronenpresse
- Puerierstab
- Sieb

## Zubereitung

Um Hummus zuzubereiten, beginnen Sie damit, Kichererbsen in ein Sieb zu geben und gründlich unter fließendem Wasser abzuspülen.
Dann pressen Sie eine Zitrone aus.

Nun können Sie alle Zutaten für den Hummus in einem Mixer vermischen.
Fügen Sie Wasser hinzu, bis alle Zutaten bedeckt sind.
Dann können Sie alles pürieren.

Sobald der Hummus die gewünschte Konsistenz erreicht hat, können Sie ihn in eine Schale geben.
Verzieren Sie den Hummus großzügig mit Olivenöl und Zatar, um ihm zusätzlichen Geschmack und Aroma zu verleihen.


## Bilder

{% assign image_dir = "/assets/images/hummus" %}
{% for image in site.static_files %}
{% if image.path contains image_dir %}
![{{ image.name }}]({{ image.path | prepend: site.baseurl }})
{% endif %}
{% endfor %}
