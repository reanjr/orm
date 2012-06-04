<?php
/**
 * Copyright Skinit, Inc.
 */

$catalog = new MysqlDataStore( 'db-dev', 'dba_user', 'foo', 'catalog' );
Orm::generateModels( $catalog, array(
    'base_dir' => 'base',
    'model_dir' => 'model',
    
    
) );