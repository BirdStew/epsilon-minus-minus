<?php
	// Introduce/reinforce PHP and HTML skills. As well as using loops (for, foreach, while) on arrays,
	// associative arrays and multidimensional arrays.  Name your file YOUR_LAST_NAME.php and require
	// this file "array_fun.php" at the top of your file to provide you the data below. ex: require "array_fun.php"; 
	// You cannot use print_r() for your output.
	//
	// #1 print $months in order - bold
	// #2 print $months in reverse order - italics
	//
	// $days_in_month: key: month name   value: days in the month
	// #3 print all keys and values ($days_in_month) = blue
	// #4 print all keys and values in key ascending order ($days_in_month) - red
	// #5 print all keys and values in value descending order ($days_in_month) - green
	// #6 print all keys and values ($days_in_month) - alternating colors
	// #7 print all keys and values ($days_in_month) where days are 31 - orange
	//
	// $seasons: key: season  value: array of month counts which is array of month and days of each month
	// #8 print each season (nested loops), under each season (indented) each month and the days
	//	in that month - various formatting
	// 		example:  	Summer
	//						#6 June (30 days)
	// 						#7 July (31 days)
	//						#8 August (31 days)
	//					Fall
	//				  		#9 September (30 days)
	//						#10 October (31 days)
	//						#11 November (30 days)
	//



	// Array of months
	$months = array(
		'January',
		'February',
		'March','April','May','June','July','August','September','October','November','December');
		
	// Associative Array of days in a month
	$days_in_month = array(
		'January' => '31',
		'February' => 28,
		'March' => 31,
		'April' => 30,
		'May' => 31,
		'June' => 30,
		'July' => 31,
		'August' => 31,
		'September' => 30,
		'October' => 31,
		'November' => 30,
		'December' => 31
	);

	// Multiple Dimension Associative Array of seasons, months and days
	$seasons  = array(
		'Winter' =>
			array(
				1 => array(
					'month' => 'January',
					'days' => 31
				),
				2 => array(
					'month' => 'February',
					'days' => 28
				),
				12 => array(
					'month' => 'December',
					'days' => 31
				)
			),
		'Spring' =>
			array(
				3 => array(
					'month' => 'March',
					'days' => 31
				),
				4 => array(
					'month' => 'April',
					'days' => 30
				),
				5 => array(
					'month' => 'May',
					'days' => 31
				)
			),
		'Summer' =>
			array(
				6 => array(
					'month' => 'June',
					'days' => 30
				),
				7 => array(
					'month' => 'July',
					'days' => 31
				),
				8 => array(
					'month' => 'August',
					'days' => 31
				)
			),
		'Fall' =>
			array(
				9 => array(
					'month' => 'September',
					'days' => 30
				),
				10 => array(
					'month' => 'October',
					'days' => 31
				),
				11 => array(
					'month' => 'November',
					'days' => 30
				)
			)
	);

?>