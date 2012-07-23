<?php
/**
 * Copyright Skinit, Inc.
 */

class Orm
{
    protected static $instance = null;
    
    protected $dataStores = array();
    
    /**
     * Singleton; no constructor access. 
     */
    protected function __construct()
    {
        // no op: singleton
    }
    
    /**
     * Return the singleton instance.  Create one if necessary.
     * 
     * @return  Orm 
     */
    public static function getInstance()
    {
        if( is_null( self::$instance ) )
        {
            self::$instance = new Orm();
        }
        
        return self::$instance;
    }
    
    /**
     * Install an instance as the singleton instance.  This should be called before any other methods in the Orm class.
     * 
     * @param   Orm     $instance
     */
    public static function installInstance( Orm $instance )
    {
        self::$instance = $instance;
    }
    
    /**
     * Register a named data store.  Setup any additional argumentes needed to identify or authenticate against the
     * data.
     * 
     * @param   string  $name
     * @param   string  $driver
     * @@param  array   $arguments
     */
    public static function registerDataStore( $name, $driver, $arguments = array() )
    {
        $instance = self::getInstance();
        $instance->dataStores[(string)$name] = array(
            "driver" => (string)$driver,
            "arguments" => $arguments,
            "instance" => null,
        );
    }
    
    /**
     * Retrieve the named data store instance.  If one does not exist, create one, call its connect method, and return
     * it.  The data store must first be registered with Orm::registerDataStore.
     * 
     * @param   string  $name 
     */
    public static function getDataStore( $name )
    {
        $name = (String)$name;
        $instance = self::getInstance();
        $dataStoreInfo = $instance->dataStores;
        
        // make sure we have a registered data store
        if( !is_array($dataStoreInfo[$name]) )
        {
            throw new OrmUnknownDataStoreException($name);
        }
        
        // create an instance if necessary
        if( is_null($dataStoreInfo[$name]["instance"]) )
        {
            $driver = $dataStoreInfo[$name]["driver"];
            if( empty($driver) )
            {
                throw new OrmException( "no registered data store '$name'" );
            }
            
            $className = ucfirst( $dataStoreInfo[$name]["driver"] ) . "DataStore";
            if( !class_exists( $className ) )
            {
                throw new OrmMissingDriverException( $dataStoreInfo[$name]["driver"] );
            }
            
            $reflector = new ReflectionClass( $className );
            $dataStoreInstance = $reflector->newInstanceArgs( $dataStoreInfo[$name]["arguments"] );
            $dataStoreInstance->connect();
            $instance->dataStores[$name]["instance"] = $dataStoreInstance;
        }
        
        // return data store instance
        return $instance->dataStores[$name]["instance"];
    }
    
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
        return (string)$name;
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