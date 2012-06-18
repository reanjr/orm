<?php
/**
 * Copyright Skinit, Inc.
 */

$configFile = file_get_contents( 'orm_config.json' );
$config = json_decode( $configFile, true );
$data = new SqliteDataStore( "test/example.db" );
Orm::generateModels( $data, $config );