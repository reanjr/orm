<?php
/**
 * Copyright Skinit, Inc.
 */

$configFile = file_get_contents( 'orm_config.json' );
$config = json_decode( $configFile, true );
$catalog = new MysqlDataStore( 'db-dev', 'dba_user', 'foo', 'catalog' );
Orm::generateModels( $catalog, $config );