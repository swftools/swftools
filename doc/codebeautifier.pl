while(<stdin>)
{
    if(/\[CALLPERL .*left\]/ ... /\[CALLPERL end\]/) {
	$code .= $_ if(!/CALLPERL/);
	$name = $1 if(/.flash.*name=([^&][^ ]*)/);
	$name = $1 if(/.flash.*name=&quot;([^&]*)&quot;/);
    } 
    elsif(/\[CALLPERL .*right\]/ ... /\[CALLPERL end\]/) {
	$highlight .= $_ if(!/CALLPERL/);
    }
    elsif ($code ne "") {
	$code =~ s/&quot;/"/g;
	open(fi, ">tmp.sc");print fi $code;close(fi);
	system("swfc tmp.sc");
	($embed = `swfdump -e $name`) =~ /WIDTH="([^"]*)"/;
	$width = $1;
	print "<td bgcolor=\"#ffffff\" width=\"$width\">";
	print $embed;
	print "</td>";
	$code="";
	print;
	unlink "tmp.sc";
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
