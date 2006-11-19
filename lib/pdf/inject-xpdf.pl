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
system("patch < ../xpdf-changes.patch") and die;
chdir("..");
system("rm -f xpdf");
system("ln -s $directory xpdf");

open(fi, ">switch");

print fi <<EOF
#!/bin/sh

if test "x\$1" = "xstable";then
    echo "Switching to stable version"
    rm -f xpdf
    ln -s xpdf-3.01 xpdf
elif test "x\$1" = "xlatest";then
    echo "Switching to latest version"
    rm -f xpdf
    ln -s $directory xpdf
fi
EOF
;
close(fi);
system("chmod a+x switch");

