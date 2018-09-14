#!/bin/bash

sudo apt -y update
sudo apt-get install -y libc6-dev g++-multilib python p7zip-full pwgen jq curl
cd ~

if [ -f zeroinsight.zip ]
then
    rm zeroinsight.zip
fi
wget -O zeroinsight.zip `curl -s 'https://api.github.com/repos/zeroinsight-pod/zeroinsight/releases/latest' | jq -r '.assets[].browser_download_url' | egrep "zeroinsight.+x64.zip"`
7z x -y zeroinsight.zip
chmod -R a+x ~/zeroinsight-pkg
rm zeroinsight.zip

cd ~/zeroinsight-pkg
./fetch-params.sh

if ! [[ -d ~/.zeroinsight ]]
then
    mkdir -p ~/.zeroinsight
fi

if ! [[ -f ~/.zeroinsight/zeroinsight.conf ]] 
then
    echo "rpcuser=rpc`pwgen 15 1`" > ~/.zeroinsight/zeroinsight.conf
    echo "rpcpassword=rpc`pwgen 15 1`" >> ~/.zeroinsight/zeroinsight.conf
fi

./zcashd