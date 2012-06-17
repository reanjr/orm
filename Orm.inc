<?php
/**
 * Copyright Skinit, Inc.
 */

class Orm
{
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
        $modelBuilder = new ModelBuilder();
        $modelBuilder->addSchema( $schematic );
        
        $basePath = array_key_exists( 'base_output_dir', $config )
          ? $config['base_output_dir']
          : '.';
        $userPath = array_key_exists( 'user_output_dir', $config )
          ? $config['user_output_dir']
          : '.';
        
        foreach( $modelBuilder->getClasses() as $class )
        {
            $className = (string)$class;
            $fileName = "$className.inc";
            $filePath = "$basePath/$fileName";
            file_put_contents( $filePath, $class->getCode() );
        }
        
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