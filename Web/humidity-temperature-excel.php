<?php
/**
 * PHPExcel
 *
 * Copyright (c) 2006 - 2015 PHPExcel
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * @category   PHPExcel
 * @package    PHPExcel
 * @copyright  Copyright (c) 2006 - 2015 PHPExcel (http://www.codeplex.com/PHPExcel) (https://github.com/PHPOffice/PHPExcel)
 * @license    http://www.gnu.org/licenses/old-licenses/lgpl-2.1.txt	LGPL
 * @version    ##VERSION##, ##DATE##
 */

$tablename = "datalog";
// Include PHPExcel
include("config-iot.php");
include("../web/PHPExcel/PHPExcel.php");
date_default_timezone_set("Asia/Kuala_Lumpur");
extract($_GET);

// Create new PHPExcel object
$objPHPExcel = new PHPExcel();

// Set document properties
$objPHPExcel->getProperties()
			->setCreator("Shahrul Nizam")
			->setLastModifiedBy("Shahrul Nizam")
			->setTitle("Humidity Temperature")
			->setSubject("Humidity Temperature")
			->setDescription("")
			->setKeywords("")
			->setCategory("");
							 
// Rename worksheet
$objPHPExcel->getActiveSheet()->setTitle('Humidity Temperature');

if(isset($_GET['id'])) $sql = "SELECT *FROM ".$tablename." WHERE status='humitempDesc' AND id='$id' LIMIT 1";
else $sql = "SELECT *FROM ".$tablename." WHERE status='humitempDesc' AND id='demo' LIMIT 1";
$result = mysqli_query($conn, $sql);
if($row = mysqli_fetch_array($result)) $description=$row['description'];
else $description='';

if(isset($_GET['id'])) $sql = "SELECT *FROM ".$tablename." WHERE status='humitempData' AND id='$id' ORDER BY timestamp DESC LIMIT 500";
else $sql = "SELECT *FROM ".$tablename." WHERE status='humitempData' AND id='demo' ORDER BY timestamp DESC LIMIT 500";

$result = mysqli_query($conn, $sql);
if($result) 
{
	$objPHPExcel->setActiveSheetIndex(0)
				->setCellValue('A1', "ID")
				->setCellValue('A2', "Description")
				->setCellValue('B1', $id)
				->setCellValue('B2', $description);			
			
	$no = 0;
	$nom=$no+4;
	$objPHPExcel->getActiveSheet()
				->setCellValue('A'.$nom, "No")
				->setCellValue('B'.$nom, "Date & Time")
				->setCellValue('C'.$nom, "Humidity (%)")
				->setCellValue('D'.$nom, "Temperature (°C)");
	$objPHPExcel->getActiveSheet()
				->getStyle('A'.$nom)
				->getAlignment()
				->setHorizontal(PHPExcel_Style_Alignment::HORIZONTAL_CENTER);	
	$objPHPExcel->getActiveSheet()
				->getStyle('B'.$nom)
				->getAlignment()
				->setHorizontal(PHPExcel_Style_Alignment::HORIZONTAL_CENTER);
	$objPHPExcel->getActiveSheet()
				->getStyle('C'.$nom)
				->getAlignment()
				->setHorizontal(PHPExcel_Style_Alignment::HORIZONTAL_CENTER);			
	$objPHPExcel->getActiveSheet()
				->getStyle('D'.$nom)
				->getAlignment()
				->setHorizontal(PHPExcel_Style_Alignment::HORIZONTAL_CENTER);				
	
	while($row = mysqli_fetch_array($result))
	{
		$no=$no+1;
		$nom=$no+4;
		$date=date("d/m/y h:i:sA",$row["timestamp"]);
		$objPHPExcel->getActiveSheet()
					->setCellValue('A'.$nom, $no)
					->setCellValue('B'.$nom, $date)
					->setCellValue('C'.$nom, $row["humidity"])
					->setCellValue('D'.$nom, $row["temperature"]);
		$objPHPExcel->getActiveSheet()
					->getStyle('A'.$nom)
					->getAlignment()
					->setHorizontal(PHPExcel_Style_Alignment::HORIZONTAL_CENTER);	
		$objPHPExcel->getActiveSheet()
					->getStyle('B'.$nom)
					->getAlignment()
					->setHorizontal(PHPExcel_Style_Alignment::HORIZONTAL_CENTER);
		$objPHPExcel->getActiveSheet()
					->getStyle('C'.$nom)
					->getAlignment()
					->setHorizontal(PHPExcel_Style_Alignment::HORIZONTAL_CENTER);			
		$objPHPExcel->getActiveSheet()
					->getStyle('D'.$nom)
					->getAlignment()
					->setHorizontal(PHPExcel_Style_Alignment::HORIZONTAL_CENTER);			
	}
}	
mysqli_close($conn);
$objPHPExcel->getActiveSheet()->getColumnDimension('A')->setAutoSize(true);
$objPHPExcel->getActiveSheet()->getColumnDimension('B')->setAutoSize(true);
$objPHPExcel->getActiveSheet()->getColumnDimension('C')->setAutoSize(true);
$objPHPExcel->getActiveSheet()->getColumnDimension('D')->setAutoSize(true);

// Redirect output to a client’s web browser (Excel2007)
header('Content-Type: application/vnd.openxmlformats-officedocument.spreadsheetml.sheet');
header('Content-Disposition: attachment;filename="IOT Humidity Temperature.xlsx"');
header('Cache-Control: max-age=0');
// If you're serving to IE 9, then the following may be needed
header('Cache-Control: max-age=1');

// If you're serving to IE over SSL, then the following may be needed
header ('Expires: Mon, 26 Jul 1997 05:00:00 GMT'); // Date in the past
header ('Last-Modified: '.gmdate('D, d M Y H:i:s').' GMT'); // always modified
header ('Cache-Control: cache, must-revalidate'); // HTTP/1.1
header ('Pragma: public'); // HTTP/1.0

$objWriter = PHPExcel_IOFactory::createWriter($objPHPExcel, 'Excel2007');
$objWriter->save('php://output');
exit;
?>
