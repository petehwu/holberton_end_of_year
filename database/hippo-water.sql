-- Drop database
DROP DATABASE IF EXISTS hippo_water_db;

-- Create database + user if doesn't exist
CREATE DATABASE IF NOT EXISTS hippo_water_db;
CREATE USER IF NOT EXISTS 'hippo_dev'@'localhost';
SET PASSWORD FOR 'hippo_dev'@'localhost' = 'hippo_dev_pwd';
GRANT ALL ON hippo_water_db.* TO 'hippo_dev'@'localhost';
GRANT SELECT ON performance_schema.* TO 'hippo_dev'@'localhost';
FLUSH PRIVILEGES;

USE hippo_water_db;

--
-- Table structure for table `amenities`
--

DROP TABLE IF EXISTS `sensor_info`;
CREATE TABLE `sensor_info` (
  `sensor_id` INTEGER NOT NULL,
  `created_at` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `updated_at` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `name` varchar(128) NOT NULL,
  PRIMARY KEY (`sensor_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;


DROP TABLE IF EXISTS `sensor_data`;
CREATE TABLE `sensor_data` (
  `id` BIGINT NOT NULL AUTO_INCREMENT,
  `sensor_id` INTEGER NOT NULL,
  `sensor_value` INTEGER NOT NULL,
  `created_at` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  FOREIGN KEY (`sensor_id`) REFERENCES `sensor_info` (`sensor_id`) ON DELETE CASCADE,
  PRIMARY KEY(`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
