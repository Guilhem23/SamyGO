<?xml version="1.0"?>
<!-- Created by ZsoltTech.Com® -->
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
	version="1.0">

	<xsl:output method="xml" indent="yes" encoding="UTF-8" />
	<xsl:template match="node() | @*">
		<xsl:copy>
			<xsl:apply-templates select="node() | @*" />
		</xsl:copy>
	</xsl:template>
	<xsl:template match="/rsp/list/widget">
		<xsl:if test="not(@id = preceding-sibling::widget/@id)">
			<xsl:copy-of select="." />
		</xsl:if>
	</xsl:template>

</xsl:stylesheet>
