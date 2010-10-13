#!/usr/bin/perl

$filename = $ARGV[0];

$filename or die "no filename";

$basename = $filename;
$basename =~ s/.tar.gz$//g;
$directory = $basename;

mkdir("$directory");
mkdir("$directory/tmp/");

chdir("$directory/tmp/") or die;

sub cleanup_and_die
{
    system("rm -f xpdf");
    system("rm -f $directory");
}

print "Extracting $filename\n";
system("tar -zxf ../../$filename") and die;
system("find . -type f -exec mv {} .. \\;") and cleanup_and_die;
chdir("..");
system("find . -type d -exec rmdir {} \\; 2> /dev/null");

print "Applying security patches...\n";
for($a=1;$a<10;$a++) {
    $patchname = "../${basename}pl$a.patch";
    if(-f $patchname) {
        print "*pl$a.patch\n";
        system("patch -s < $patchname") and cleanup_and_die;
    }
}

$c = 'find . \( -name "*.cc" -or -name "*.h" -or -name "*.c" \) -exec cp {} {}.orig \;';
print "$c\n";
system($c);

system("patch < ../xpdf-changes.patch 2>&1 | grep -i failed");
chdir("..");
system("rm -f xpdf");
system("ln -s $directory xpdf");

system("touch xpdf/*");

open(fi, ">switch");

print fi <<EOF
#!/bin/sh

VERSION=\$1
PS3="choose> "

if test "x\$VERSION" = "x";then
    select V in stable latest;do VERSION="\$V";break;done
fi
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


