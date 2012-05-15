<?php
/**
 * Copyright Skinit, Inc.
 */

class Orm
{
    const APP_WEBSITE = 'website';
    const APP_CATALOG = 'catalog';
    const APP_PAYMENT = 'payment';
    
    const ERROR_INVALID_PRIMARY_KEY = 'invalid primary key';
    
    /**
     * Generate model classes from the INFORMATION_SCHEMA of the provided database.
     * 
     * Config:
     * {
     *      "app": "catalog",
     *      "base_path": "models/orm/base",
     *      "user_path": "models/orm"
     * }
     * 
     * @param   Db      $db
     * @param   array   $config
     */
    public static function generateModels( Db $db, array $config )
    {
        
    }
    
    /**
     * Return the name of the current application.  This should be one of the Orm::APP_* constants.  Models for
     * applications that are different than the current one will act as remote models, while those that share the
     * current application will act as local models.
     * 
     * @return  string 
     */
    public static function getCurrentApplication()
    {
        
    }
    
}