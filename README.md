# metaFx
La metaFx est une surface de contrôle de transformations continues du son en temps réel au même titre que la metaSurface64. Comme elle, elle dispose à la fois de son propre générateur de boucles et d'un moteur FX multieffets, mais son fonctionnement est différent, en particulier pour la gestion des chaînes de greffons et les pavés.

Chaque mixeur de cette surface peut permettre de contrôler directement le gain et les greffons attachés. Il est possible également de contrôler les pistes d'un séquenceur externe (Ardour ou Reaper). Pour ce faire, l'application utilise des modules contrôlables par OSC qui proviennent de la librairie du langage Faust qui est embarqué dans l'application.

Ce programme a été testé avec Jack sous Linux / LibraZiK2.

Pour intaller cette application :

git clone https://github.com/dblanchemain/metaFx

make metaFx

make install PREFIX=/usr/local

puis pour Linux :

./metaFx.sh

La documentation est disponible ici :

http://blanchemain.info/Documents/Programmation/index.php?page=metaFx

Cette version 0.1 a été réalisée et testée sur debian/LibraZik2