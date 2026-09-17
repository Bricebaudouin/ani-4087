mon tableau est constitué comme suit: 

| Étape | Valeur trouvée | Source |

-  Les capteurs mesurent le mouvement / Capteur IMU à 1000 Hz → ~2 ms entre le mouvement et la réception par le moteur (regroupe capteur + transmission) / Meta/Oculus, "Building a Sensor for Low Latency VR" — meta.com/blog/building-a-sensor-for-low-latency-vr 

-  Le système transmet la mesure / Valeur non trouvée.

- Votre application décide et dessine / Rendu estimé entre 2 et 10 ms ; borne théorique idéale de 7 ms pour tout le pipeline / AI Enabled 6G for Semantic Metaverse, arXiv 2507.19124 ; Michael Abrash, "Latency – the sine qua non of AR and VR", blog Valve (2012)
  
-  Le compositeur assemble / Valeur en ms non trouvée ; seule la mécanique (distorsion/warp avant affichage) est documentée / developer.oculus.com, "Asynchronous TimeWarp (ATW)" .
   
- L'écran affiche la ligne / Temps de réponse pixel OLED "bien moins d'une milliseconde"  / oled-info.com, couverture CES du prototype "Crystal Cove" |
