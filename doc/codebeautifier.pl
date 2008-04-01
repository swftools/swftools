$nr = 1;
while(<stdin>)
{
    if(/\[CALLPERL\s+(.*)\s+left\]/ ... /\[CALLPERL end\]/) {
	$lang = $1 if($1);
	$code .= $_ if(!/CALLPERL/);
	$name = $1 if(/.flash.*name=([^&][^ ]*)/);
	$name = $1 if(/.flash.*name=&quot;([^&]*)&quot;/);
	chomp $name;
    } 
    elsif(/\[CALLPERL .*right\]/ ... /\[CALLPERL end\]/) {
	$highlight .= $_ if(!/CALLPERL/);
    }
    elsif ($code ne "" && ($lang eq "swfc" || $lang eq "sc")) {
	$code =~ s/&quot;/"/g;

#	    print stderr "Warning: ttf->swf\n" if($code =~ s/Arial.ttf/Arial.swf/g);

	open(fi, ">tmp.sc");print fi $code;close(fi);
	print stderr "[$lang] swfc tmp.sc ($name)\n";
	system("../src/swfc tmp.sc >&2");
	($embed = `swfdump -e $name`) =~ /WIDTH="([^"]*)"/;
	system("cp $name data");
	$width = $1;
	print "<td bgcolor=\"#ffffff\" width=\"$width\">";
	print $embed;
	print "</td>";
	$code="";
	print;
	unlink "tmp.sc";
    }
    elsif ($code ne "" && ($lang eq "python" or $lang eq "shell")) {
	$code =~ s/&quot;/"/g;
	if($lang eq "python") {
	   open(fi, ">$nr.py");print fi $code;close(fi);
	    $nr = $nr + 1;
	}
	print "<td bgcolor=\"#ffffff\" width=\"$width\">";
	print $embed;
	print "</td>";
	$code="";
	print;
    }
    elsif ($code ne "") {
	$code="";
    }
    elsif ($highlight ne "") {
	$highlight =~ s/^\n\s*//g;
	$highlight =~ s/\s*\n$//g;
	print "<pre>\n";
	# todo: apply syntax highlighting.
	print $highlight."\n";
	print "</pre>\n";
	$highlight="";
	print;
    }
    else {
	print;
    }
}
