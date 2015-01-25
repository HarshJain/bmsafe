Le fichier "eRacing_sfe-gerb274x.cam" est un script d'exportation d'un layout eagle en fichiers Gerbers.

-- Tutoriel --
Ce script est bas� sur le script de sparkfun (voir le tuto : https://www.sparkfun.com/tutorials/115).


--- Diff�rence entre les scripts ---
La seule diff�rence avec le script original est l'ajout de la couche tNames, en plus de tPlace sur le Top Silkscreen et
l'ajout de la couche bNames en plus de bPlace sur la couche Bottom Silkscreen.


--- Utilisation ---

(Facultatif) - Copier le script dans le dossier de eagle contenant les "cam jobs" ("Program Files/EagleXX/cam").
- Dans eagle : Menu File -> CAM processor...
- Dans la fen�tre qui s'ouvre, aller dans le Menu File -> Open -> Job...
- Choisir le script "eRacing_sfe-gerb274x.cam", puis l'ouvrir.
- Les onglets qui vont appara�tre correspondent � chaque fichier gerber qui sera g�n�r�.
- Appuyer sur le bouton "Process Job" permet de g�n�rer tous les Gerbers. L'op�ration peut prendre du temps selon la complexit� du circuit
(~25 minutes pour le BMS Slave).


--- Fichiers Gerbers ----
Les fichiers Gerbers g�n�r�s (dans le dossier de votre projet) sont les suivants :

.GTL : Top Copper (Top + Pads + Vias + Dimensions)
.GBL : Bottom Copper (Bottom + Pads + Vias)
.GTO : Top Silkscreen (tPlace + tName)
.GBO : Bottom Silkscreen (bPlace + bName)
.TXT : Drill file (Drills + Holes)
.GTS : Top Soldermask (tStop)
.GBS : Bottom Soldermask (bStop)
.GTP : Top Paste (tPaste)
.GML : Mill Layer

Parmi ces neuf fichiers, les 7 premiers sont les plus important.
Le .GTP sert pour faire un pochoir pour l'assemblage (je crois).
Le .GML sert �... je ne sais pas, de toute fa�on il est vide.
