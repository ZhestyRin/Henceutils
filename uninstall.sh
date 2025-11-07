#!/bin/bash

sudo -v

man_path() {

	manpath | cut -d ":" -f1

}

sudo rm -f /usr/local/bin/lf
sudo rm -f $(man_path)/man1/lf.1.gz

echo "Uninstalled Succesfully"
