-- phpMyAdmin SQL Dump
-- version 4.5.4.1deb2ubuntu2
-- http://www.phpmyadmin.net
--
-- Host: localhost
-- Generation Time: Nov 24, 2017 at 04:49 PM
-- Server version: 5.7.20-0ubuntu0.16.04.1
-- PHP Version: 7.0.22-0ubuntu0.16.04.1

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Database: `chessClub`
--
CREATE DATABASE IF NOT EXISTS `chessClub` DEFAULT CHARACTER SET latin1 COLLATE latin1_swedish_ci;
USE `chessClub`;

-- --------------------------------------------------------

--
-- Table structure for table `bughousechat`
--

CREATE TABLE IF NOT EXISTS `bughousechat` (
  `Game_ID` int(11) NOT NULL,
  `Game_ID2` int(11) NOT NULL,
  `Username` text NOT NULL,
  `Message` text NOT NULL,
  `Teammate` text NOT NULL,
  `Time` bigint(20) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `Chat`
--

CREATE TABLE IF NOT EXISTS `Chat` (
  `Username` text NOT NULL,
  `Message` text NOT NULL,
  `Time` int(11) NOT NULL
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `chessgame`
--

CREATE TABLE IF NOT EXISTS `chessgame` (
  `Turn` varchar(1) NOT NULL DEFAULT 'w',
  `WTime` int(11) NOT NULL,
  `BTime` int(11) NOT NULL,
  `Undo_Time` int(11) DEFAULT '0',
  `White` text NOT NULL,
  `Black` text NOT NULL,
  `ID` int(11) NOT NULL AUTO_INCREMENT,
  `Winner` text,
  `BHID` int(11) DEFAULT NULL,
  `Type` text NOT NULL,
  `LID` int(11) NOT NULL DEFAULT '-1',
  `I` int(11) NOT NULL DEFAULT '0',
  `turns` int(11) NOT NULL DEFAULT '0',
  `Time` bigint(20) NOT NULL DEFAULT '0',
  `WEP` varchar(6) NOT NULL DEFAULT 'false',
  `BEP` varchar(6) NOT NULL DEFAULT 'false',
  `Castle` varchar(8) NOT NULL DEFAULT 'wkwqbkbq',
  `Last_Taken` varchar(1) DEFAULT 'X',
  `Promote` tinyint(4) NOT NULL DEFAULT '0',
  `Draw_Offered` int(11) NOT NULL DEFAULT '0',
  `White_Captured` text,
  `Black_Captured` text,
  `Board` text NOT NULL,
  `moves` text,
  `Past` text,
  PRIMARY KEY (`ID`),
  KEY `ID` (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `Draw`
--

CREATE TABLE IF NOT EXISTS `Draw` (
  `Game_ID` int(11) NOT NULL,
  `Accepted_by_White` int(11) NOT NULL,
  `Accepted_by_Black` int(11) NOT NULL
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `gamechat`
--

CREATE TABLE IF NOT EXISTS `gamechat` (
  `Game_ID` int(11) NOT NULL,
  `Username` text NOT NULL,
  `Message` text NOT NULL,
  `Time` bigint(20) NOT NULL
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `lobby`
--

CREATE TABLE IF NOT EXISTS `lobby` (
  `Owner` text NOT NULL,
  `Type` text NOT NULL,
  `Player1` text,
  `Player2` text,
  `Player3` text,
  `Player4` text,
  `Players` int(2) NOT NULL DEFAULT '1',
  `Time` text,
  `I` int(11) NOT NULL DEFAULT '0',
  `ID` int(10) UNSIGNED NOT NULL AUTO_INCREMENT,
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `users`
--

CREATE TABLE IF NOT EXISTS `users` (
  `Username` text NOT NULL,
  `First_Name` text NOT NULL,
  `Last_Name` text NOT NULL,
  `Email` text NOT NULL,
  `Played` int(11) NOT NULL DEFAULT '0',
  `Won` int(11) NOT NULL DEFAULT '0',
  `Draw` int(11) NOT NULL DEFAULT '0',
  `Lost` int(11) NOT NULL DEFAULT '0',
  `BPlayed` int(11) NOT NULL DEFAULT '0',
  `BWon` int(11) NOT NULL DEFAULT '0',
  `BLost` int(11) NOT NULL DEFAULT '0',
  `BDraw` int(11) NOT NULL DEFAULT '0',
  `Time` bigint(20) NOT NULL,
  `Type` varchar(1) NOT NULL DEFAULT 'U',
  `ID` int(11) NOT NULL AUTO_INCREMENT,
  `Password` text NOT NULL,
  `Salt` text NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
