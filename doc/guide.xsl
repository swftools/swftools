<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
<!--
   SWFTools documentation stylesheet.

   Major portions shamelessly stolen from the gentoo documentation project (www.gentoo.org).

   This file is distributed under the GPL, see file COPYING for details.
-->
  <xsl:output encoding="iso-8859-15" method="html" indent="yes" doctype-public="-//W3C//DTD HTML 4.01 Transitional//EN"/>
  <xsl:preserve-space elements="code"/>
  <xsl:template match="img">
      <img src="{@src}"/>
  </xsl:template>

  <xsl:template match="/guide">
	<html>
	<head>
	 <link title="new" rel="stylesheet" href="caiviar.css" type="text/css"/>
	 <link REL="shortcut icon" HREF="/favicon.ico" TYPE="image/x-icon"/>
	 <title>
	 <xsl:value-of select="title"/>
	 </title>
	</head>
	<body style="margin-left:0px;margin-top:0px;" bgcolor="#ffffff">
	 <table>
	  <tr>
	   <td>
		<h1>
		 <b>
		 <font class="dochead">
		    <xsl:value-of select="title"/>
		 </font>
	 	 </b>
		</h1>
	   </td>
	  </tr>
	  <tr>
	   <td>
	     <img src="line.gif" alt="line"/>
	   </td>
	  </tr>
	  <tr>
	   <td class="alttext"><b>Summary:</b>&#160;<xsl:apply-templates select="abstract"/></td>
	  </tr>
	  <tr>
	   <td>
	     <img src="line.gif" alt="line"/>
	   </td>
	  </tr>
	</table>

	<xsl:apply-templates select="chapter"/>
	</body>
	</html>
  </xsl:template>
    
  <xsl:template match="mail">
    <a href="mailto:{@link}">
      <xsl:value-of select="."/>
    </a>
  </xsl:template>

  <xsl:template match="chapter">
    <xsl:variable name="chid"><xsl:number/></xsl:variable>
    <xsl:choose>
      <xsl:when test="title">
	<p class="chaphead">
	  <font class="chapnum">
	    <a name="doc_chap{$chid}"><xsl:number/>.</a>
	  </font>
	  <xsl:value-of select="title"/>
	</p>
      </xsl:when>
      <xsl:otherwise>
	<p class="chaphead">
	  <font class="chapnum">
	    <a name="doc_chap{$chid}"><xsl:number/>.</a>
	  </font>
	</p>
      </xsl:otherwise>
    </xsl:choose>
    <xsl:apply-templates>
      <xsl:with-param name="chid" select="$chid"/>
    </xsl:apply-templates>
  </xsl:template>

  <xsl:template match="section">
    <xsl:param name="chid"/>
    <xsl:variable name="sid"><xsl:number/></xsl:variable>
    <xsl:if test="title">
      <p class="secthead">
        <xsl:value-of select="$chid"/>.<xsl:value-of select="$sid"/>&#160;
	<a name=""><xsl:value-of select="title"/>&#160;</a>
      </p>
    </xsl:if>
    <xsl:apply-templates>
      <xsl:with-param name="chid" select="$chid"/>
      <xsl:with-param name="sid" select="$sid"/>
    </xsl:apply-templates>
  </xsl:template>

  <xsl:template match="subsection">
    <xsl:param name="chid"/>
    <xsl:param name="sid"/>
    <xsl:variable name="ssid"><xsl:number/></xsl:variable>
    <xsl:if test="title">
      <p class="subsecthead">
        <xsl:value-of select="$chid"/>.<xsl:value-of select="$sid"/>.<xsl:value-of select="$ssid"/>&#160;
	<a name=""><xsl:value-of select="title"/>&#160;</a>
      </p>
    </xsl:if>
    <xsl:apply-templates>
      <xsl:with-param name="chid" select="$chid"/>
      <xsl:with-param name="sid" select="$sid"/>
      <xsl:with-param name="ssid" select="$ssid"/>
    </xsl:apply-templates>
  </xsl:template>

  <xsl:template match="subsubsection">
    <xsl:param name="chid"/>
    <xsl:param name="sid"/>
    <xsl:param name="ssid"/>
    <xsl:variable name="sssid"><xsl:number/></xsl:variable>
    <xsl:if test="title">
      <p class="subsubsecthead">
        <xsl:value-of select="$chid"/>.<xsl:value-of select="$sid"/>.<xsl:value-of select="$ssid"/>.<xsl:value-of select="$sssid"/>&#160;
	<a name=""><xsl:value-of select="title"/>&#160;</a>
      </p>
    </xsl:if>
    <xsl:apply-templates>
      <xsl:with-param name="chid" select="$chid"/>
      <xsl:with-param name="sid" select="$sid"/>
      <xsl:with-param name="ssid" select="$ssid"/>
      <xsl:with-param name="sssid" select="$sssid"/>
    </xsl:apply-templates>
  </xsl:template>

  <xsl:template match="figure">
    <xsl:with-param name="chid"/>
    <xsl:variable name="fignum">
      <xsl:number level="any" from="chapter" count="figure"/>
    </xsl:variable>
    <xsl:variable name="figid">doc_chap<xsl:value-of select="$chid"/>_fig<xsl:value-of select="$fignum"/></xsl:variable>
    <br/>
    <a name="{$figid}"/>
    <table cellspacing="0" cellpadding="0" border="0">
      <tr>
	<td class="infohead" bgcolor="#7a5ada">
	  <p class="caption">
	    <xsl:choose>
	      <xsl:when test="@caption">
				Figure <xsl:value-of select="$chid"/>.<xsl:value-of select="$fignum"/>: <xsl:value-of select="@caption"/>
			</xsl:when>
	      <xsl:otherwise>
				Figure <xsl:value-of select="$chid"/>.<xsl:value-of select="$fignum"/>
			</xsl:otherwise>
	    </xsl:choose>
	  </p>
	</td>
      </tr>
      <tr>
	<td align="center" bgcolor="#ddddff">
	  <xsl:choose>
	    <xsl:when test="@short">
	      <img src="{@link}" alt="Fig. {$fignum}: {@short}"/>
	    </xsl:when>
	    <xsl:otherwise>
	      <img src="{@link}" alt="Fig. {$fignum}"/>
	    </xsl:otherwise>
	  </xsl:choose>
	</td>
      </tr>
    </table>
    <br/>
  </xsl:template>

  <!--figure without a caption; just a graphical element-->
  <xsl:template match="fig">
    <center>
      <xsl:choose>
	<xsl:when test="@linkto">
	  <a href="{@linkto}">
	    <img src="{@link}" alt="{@short}"/>
	  </a>
	</xsl:when>
	<xsl:otherwise>
	  <img src="{@link}" alt="{@short}"/>
	</xsl:otherwise>
      </xsl:choose>
    </center>
  </xsl:template>

  <xsl:template match="br">
    <br/>
  </xsl:template>

  <xsl:template match="note">
    <table class="ncontent" width="100%" border="0" cellspacing="0" cellpadding="0">
      <tr>
	<td bgcolor="#bbffbb">
	  <p class="note">
	    <b>Note: </b>
	    <xsl:apply-templates/>
	  </p>
	</td>
      </tr>
    </table>
  </xsl:template>

  <xsl:template match="impo">
    <table class="ncontent" width="100%" border="0" cellspacing="0" cellpadding="0">
      <tr>
	<td bgcolor="#ffffbb">
	  <p class="note">
	    <b>Important: </b>
	    <xsl:apply-templates/>
	  </p>
	</td>
      </tr>
    </table>
  </xsl:template>

  <xsl:template match="warn">
    <table class="ncontent" width="100%" border="0" cellspacing="0" cellpadding="0">
      <tr>
	<td bgcolor="#ffbbbb">
	  <p class="note">
	    <b>Warning: </b>
	    <xsl:apply-templates/>
	  </p>
	</td>
      </tr>
    </table>
  </xsl:template>

  <xsl:template match="codenote">
    <font class="comment">// <xsl:value-of select="."/></font>
  </xsl:template>

  <xsl:template match="comment">
    <font class="comment">
      <xsl:apply-templates/>
    </font>
  </xsl:template>

  <xsl:template match="i">
    <font class="input">
      <xsl:apply-templates/>
    </font>
  </xsl:template>

  <xsl:template match="b">
    <b>
      <xsl:apply-templates/>
    </b>
  </xsl:template>

  <xsl:template match="brite">
    <font class="brite">
      <b>
	<xsl:apply-templates/>
      </b>
    </font>
  </xsl:template>

  <xsl:template match="c">
    <font class="code">
      <xsl:apply-templates/>
    </font>
  </xsl:template>

  <xsl:template match="box">
    <p class="infotext">
      <xsl:apply-templates/>
    </p>
  </xsl:template>

  <xsl:template match="shell">
    <table class="ntable" width="100%" cellspacing="0" cellpadding="0" border="0">
      <tr>
	<td bgcolor="#f0f0f0">
	  <pre>
	    <font><xsl:apply-templates/></font>
	  </pre>
	</td>
      </tr>
    </table>
    <br/>
  </xsl:template>

  <xsl:template match="property">
    <table class="itable" width="100%" cellspacing="0" cellpadding="0" border="0">
    <tr><td width="2%"></td><td></td></tr> <!-- FIXME -->
      <tr>
        <td width="2%">
	</td>
	<td>
	    <font><b><xsl:apply-templates/></b></font>
	</td>
      </tr>
    <tr></tr>
    <tr><td width="2%"></td><td></td></tr> <!-- FIXME -->
    </table>
  </xsl:template>

  <xsl:template match="code">
    <xsl:param name="chid"/>
    <xsl:variable name="prenum">
      <xsl:number level="any" from="chapter" count="code"/>
    </xsl:variable>
    <xsl:variable name="preid">doc_chap<xsl:value-of select="$chid"/>_pre<xsl:value-of select="$prenum"/></xsl:variable>
    <a name="{$preid}"/>
    <table class="ntable" width="100%" cellspacing="0" cellpadding="0" border="0">
      <tr>
        <td></td><td width="32"></td>
	<td class="infohead" bgcolor="#6c6cff">
	  <p class="caption">
	    <xsl:choose>
	      <xsl:when test="@caption">
			Code listing <xsl:value-of select="$chid"/>.<xsl:value-of select="$prenum"/>: <xsl:value-of select="@caption"/>
		</xsl:when>
	      <xsl:otherwise>
			Code listing <xsl:value-of select="$chid"/>.<xsl:value-of select="$prenum"/>
		</xsl:otherwise>
	    </xsl:choose>
	  </p>
	</td>
      </tr>
      <tr>
      <!-- <td> is inserted by perl -->
[CALLPERL <xsl:value-of select="@lang"/> left]
<xsl:apply-templates/>
[CALLPERL end]
      <!-- </td> is inserted by perl -->
        <td width="32">
        </td>
	<td bgcolor="#e0ffff" valign="top">
[CALLPERL <xsl:value-of select="@lang"/> right]
<xsl:apply-templates/>
[CALLPERL end]
	</td>
      </tr>
    </table>
    <br/>
  </xsl:template>

  <xsl:template match="f">
    <font class="path">
      <xsl:value-of select="."/>
    </font>
  </xsl:template>

  <xsl:template match="a">
  <!-- expand templates to handle things like <a href="http://bar"><c>foo</c></a> -->
    <xsl:choose>
      <xsl:when test="@href">
	<a href="{@href}">
	  <xsl:apply-templates/>
	</a>
      </xsl:when>
      <xsl:otherwise>
	<xsl:variable name="loc" select="."/>
	<a href="{$loc}">
	  <xsl:apply-templates/>
	</a>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template match="p">
    <xsl:param name="chid"/>
      <xsl:choose>
      <xsl:when test="@class">
		<p class="{@class}">
		<xsl:apply-templates>
		<xsl:with-param name="chid" select="$chid"/>
		</xsl:apply-templates>
		</p>
	</xsl:when>
	<xsl:otherwise>
		<p>
		<xsl:apply-templates>
		<xsl:with-param name="chid" select="$chid"/>
		</xsl:apply-templates>
		</p>
	</xsl:otherwise>
	</xsl:choose>
  </xsl:template>

  <xsl:template match="title">
  </xsl:template>

  <xsl:template match="e">
    <font class="emphasis">
      <xsl:apply-templates/>
    </font>
  </xsl:template>

  <xsl:template match="mail">
    <a href="mailto:{@link}">
      <xsl:value-of select="."/>
    </a>
  </xsl:template>

  <xsl:template match="table">
    <table class="ntable">
      <xsl:apply-templates/>
    </table>
  </xsl:template>

  <xsl:template match="tr">
    <tr>
      <xsl:apply-templates/>
    </tr>
  </xsl:template>

  <xsl:template match="ti">
    <td bgcolor="#cccccc" class="tableinfo">
      <xsl:apply-templates/>
    </td>
  </xsl:template>

  <xsl:template match="th">
    <td bgcolor="#6c6cff" class="infohead">
      <b>
	<xsl:apply-templates/>
      </b>
    </td>
  </xsl:template>

  <xsl:template match="ul">
    <ul>
      <xsl:apply-templates/>
    </ul>
  </xsl:template>

  <xsl:template match="ol">
    <ol>
      <xsl:apply-templates/>
    </ol>
  </xsl:template>

  <xsl:template match="li">
    <li>
      <xsl:apply-templates/>
    </li>
  </xsl:template>

<!--  <xsl:template match="text()">
   [
   <xsl:value-of select="value()"/>
   ]
  </xsl:template>
-->
</xsl:stylesheet>
