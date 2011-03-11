<?

if(isset($_GET['pageNo'])){
	echo "<table>";
	for($no=1;$no<=10;$no++){
		$counter = $_GET['pageNo'] * 10 + $no;
		
		echo "<tr valign=\"top\"><td>$counter &nbsp;</td><td><b>Heading for item $counter</b></td></tr>";
		echo "<tr><td></td><td>More text is comming here.More text is comming here.More text is comming here.More text is comming here.More text is comming here.More text is comming here.</td></tr>";		
	}	
	
	echo "<tr>";
	echo "<td colspan=\"2\">";	
	if($_GET['pageNo']>0){
		echo "<a href=\"#\" onclick=\"dynPageObj.setUrl('getPage.php?pageNo=".($_GET['pageNo']-1)."');dynPageObj.loadPage();this.style.display='none';return false\">Previous page</a> ";		
	}
	echo "<a href=\"#\" onclick=\"dynPageObj.setUrl('getPage.php?pageNo=".($_GET['pageNo']+1)."');dynPageObj.loadPage();this.style.display='none';return false\">Next page</a> ";
	echo "</td>";
	echo "</tr>";
	echo "</table>";
}


?>
