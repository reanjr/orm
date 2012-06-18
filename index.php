<?php
/**
 * Copyright Skinit, Inc.
 */

require_once( 'autoload.inc' );

// read config
$configFile = file_get_contents( 'orm_config.json' );
$config = json_decode( $configFile, true );

// connect to data store
$data = new SqliteDataStore( "test/example.db" );
$data->connect();

// create model files
Orm::generateModels( $data, $config );