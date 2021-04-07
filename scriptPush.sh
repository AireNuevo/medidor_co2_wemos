#! /bin/bash
echo "Ingresa el mensaje del commit"
read msg
git add . 
git commit -m"${msg}"
git push