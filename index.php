<?php
/**
 * Copyright Skinit, Inc.
 */

require_once( 'autoload.inc' );

// read config
$configFile = file_get_contents( 'orm_config.json' );
$config = json_decode( $configFile, true );

// register the data store and get its instance
Orm::registerDataStore( "Example", "sqlite", array( "test/example.db" ) );
$dataStore = Orm::getDataStore( "Example" );

// create model files
Orm::generateModels( $dataStore, $config );
