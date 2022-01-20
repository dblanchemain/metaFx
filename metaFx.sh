#! /bin/sh

if [ -d $HOME/.config/metaFx ] ; then 

	echo "Le dossier de travail ($HOME)/metaFx existe" 

else 

	mkdir $HOME/.config/metaFx
	cp  /usr/local/share/metaFx/preferences.spc $HOME/.config/metaFx/

fi 

/usr/local/bin/metaFx /usr/local/share/metaFx $HOME
