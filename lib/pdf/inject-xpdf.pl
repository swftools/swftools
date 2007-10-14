#!/usr/bin/perl

$filename = $ARGV[0];

$filename or die "no filename";

$directory = $filename;
$directory =~ s/.tar.gz$//g;

mkdir("$directory");
mkdir("$directory/tmp/");

chdir("$directory/tmp/") or die;
system("tar -zxvf ../../$filename") and die;
system("find -type f -exec mv {} .. \\;") and die;
chdir("..");
system("find -type d -exec rmdir {} \\; 2> /dev/null");
system("patch < ../xpdf-changes.patch");
chdir("..");
system("rm -f xpdf");
system("ln -s $directory xpdf");

open(fi, ">switch");

print fi <<EOF
#!/bin/sh

VERSION=\$1
PS3="choose> "

if test "x\$VERSION" = "x";then
    select V in stable latest;do VERSION="\$V";break;done
fi
case $VERSION

if test "x\$VERSION" = "xstable";then
    echo "Switching to stable version"
    rm -f xpdf
    ln -s xpdf-3.02 xpdf
elif test "x\$VERSION" = "xlatest";then
    echo "Switching to latest version"
    rm -f xpdf
    ln -s $directory xpdf
fi
EOF
;
close(fi);
system("chmod a+x switch");

