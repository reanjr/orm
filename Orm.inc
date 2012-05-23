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
     * @param   ISchematic      $schematic
     * @param   array           $config
     */
    public static function generateModels( ISchematic $schematic, array $config )
    {
        foreach( $schematic->getRelations() as $relation )
        {
            // generate model class names from the relation name
            $className = ucfirst( self::formatLogicalName( $relation ) ) . "Model";
            $baseClassName = $className . "Base";

            // determine what properties the class needs from the relation attributes
            $properties = array();
            foreach( $schematic->getRelationAttributes( $relation ) as $attribute )
            {
                $propertyName = self::formatLogicalName( $attribute );
                $type = $schematic->getRelationAttributeType( $relation, $attribute );
                $properties[$propertyName] = $type;
            }
        }
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
    
    /**
     * Return a formatted physical name for the provided "physical_name", "logicalName", or "friendly name".
     * 
     * @param   string  $name
     * @return  string
     */
    public static function formatPhysicalName( $name )
    {
        // if there's an underscore in the name, assume it's already a physical name
        if( preg_match( '@_@', $name ) )
        {
            return $name;
        }
        
        // if there's a space in the name, assume it's in friendly format
        if( preg_match( '@ @', $name ) )
        {
            return strtolower( str_replace( ' ', '_', $name ) );
        }
        
        // otherwise, assume it's in logical format
        $logicalWords = preg_split( '@([[:upper:]][[:lower:]]+)@', $name, null,
                PREG_SPLIT_DELIM_CAPTURE|PREG_SPLIT_NO_EMPTY );     // identify Word as delimiter, capture words
        return strtolower( implode( '_', $logicalWords ) );
    }
    
    /**
     * Return a logical name for the provided "physical_name", "logicalName", or "friendly name".
     * 
     * @param   string  $name
     * @return  string 
     */
    public static function formatLogicalName( $name )
    {
        // if there's an underscore in the name, assume it's in physical name
        if( preg_match( '@_@', $name ) )
        {
            $physicalWords = explode( '_', strtolower($name) );
            return lcfirst( implode( '', array_map('ucfirst', $physicalWords) ) );
        }
        
        // if there's a space in the name, assume it's in friendly format
        if( preg_match( '@ @', $name ) )
        {
            $friendlyWords = explode( ' ', strtolower($name) );
            return lcfirst( implode( '', array_map('ucfirst', $friendlyWords) ) );
        }
        
        // otherwise, assume it's already in logical format
        return $name;
    }
    
    /**
     * Return a friendly name for the provided "physical_name", "logicalName", or "friendly name".
     * 
     * @param   string  $name
     * @return  string
     */
    public static function formatFriendlyName( $name )
    {
        // if there's an underscore in the name, assume it's in physical name
        if( preg_match( '@_@', $name ) )
        {
            $physicalWords = explode( '_', strtolower($name) );
            return ucwords( implode( ' ', $physicalWords ) );
        }
        
        // if there's a space in the name, assume it's already in friendly format
        if( preg_match( '@ @', $name ) )
        {
            return $name;
        }
        
        // otherwise, assume it's in logical format
        $logicalWords = preg_split( '@([[:upper:]][[:lower:]]+)@', $name, null,
                PREG_SPLIT_DELIM_CAPTURE|PREG_SPLIT_NO_EMPTY );     // identify Word as delimiter, capture words
        return ucwords( strtolower( implode( ' ', $logicalWords ) ) );
    }
    
}