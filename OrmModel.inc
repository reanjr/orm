<?php
/**
 * Copyright Skinit, Inc.
 */

abstract class OrmModel
{
    protected $materialized = false;
    protected $hydrated = false;
    
    /** @var    array   $properties     the current model property values */
    private $properties = array();
    
    /** @var    array   $modified       the list of properties that have been modified */
    private $modified = array();
    
    /**
     * If values are provided, set the property values for the corresponding fields of the primary key.  Otherwise,
     * set new to true.  Compound primary keys can be set by passing extra arguments.
     * 
     * @param   int|string|bool     $primaryKey_1
     */
    public function __construct( $primaryKey_1 )
    {
        $keyNames = $this->getPrimaryKey();
        $keyValues = func_get_args();
        
        // this instance is materialized to the data store if a PK was provided
        $this->materialized = (bool)$keyValues;
        
        // ensure the number of provided key values matches the number of values in the PK
        if( count($keyValues) != count($keyNames) )
        {
            throw new Exception( Orm::ERROR_INVALID_PRIMARY_KEY );
        }
        
        // set PK properties
        $keyValues = array_combine( $keyNames, $keyValues );
        foreach( $keyValues as $field => $value )
        {
            $this->properties[$field] = $value;
        }
    }
    
    /**
     * Create a new instance, hydrate it from the data store based on the provided primary key values, and return the
     * instance.  Compound primary keys can be looked up by passing extra arguments.
     * 
     * @param   int|string|bool     $primaryKey_1
     */
    public static function getByPrimaryKey( $primaryKey_1 )
    {
        $keyNames = $this->getPrimaryKey();
        $keyValues = func_get_args();
        
        // ensure the number of provided key values matches the number of values in the PK
        if( count($keyValues) != count($keyNames) )
        {
            throw new Exception( Orm::ERROR_INVALID_PRIMARY_KEY );
        }
        
        // set PK properties
    }
    
    /**
     * Return the application for the model.  This should be one of the Orm::APP_* constants.
     * 
     * @return  string
     */
    abstract public function getApplication();
    
    /**
     * Return the schema name for this model's data store. 
     * 
     * @return  string
     */
    abstract public function getSchema();
    
    /**
     * Return the table name for this model's data store.
     * 
     * @return  string
     */
    abstract public function getTable();
    
    /**
     * Return the keys for the model.  This includes the primary key and any additional unique keys.  Each key is a
     * tuple of field names.
     * 
     * @return  array
     */
    public function getKeys()
    {
        return array_merge( $this->getPrimaryKey(), $this->getUniqueKeys() );
    }
    
    /**
     * Return the primary key for the model.  This is a tuple of field names.
     * 
     * @return  array 
     */
    abstract public function getPrimaryKey();
    
    /**
     * Return unique keys for the model.  These are each a tuple of field names.  This should not include the primary
     * key.
     * 
     * @return  array 
     */
    abstract public function getUniqueKeys();
    
    /**
     * Return true if this model's application does not match the current application.  Otherwise, return false.
     * 
     * @return  bool
     */
    public function isRemote()
    {
        return $this->getApplication() != Orm::getCurrentApplication();
    }
    
    /**
     * Return true if this model's application matches the current application.  Otherwise, return false.
     * 
     * @return  bool 
     */
    public function isLocal()
    {
        return $this->getApplication() == Orm::getCurrentApplication();
    }
    
    /**
     * Return true if any of the properties of this model have been updated since the last time it was saved.  Otherwise
     * return false.
     * 
     * @return  bool
     */
    public function isModified()
    {
        return count($this->modified) > 0;
    }
    
    /**
     * Return true if this model's properties have been refreshed from the data store.  This is typically false when
     * dealing with related models.  These related models will only have the PK populated.  Other properties will be
     * loaded from the data store on demand when the first non-PK property is read.
     * 
     * @return  bool
     */
    public function isHydrated()
    {
        return $this->hydrated;
    }
    
    /**
     * Return true if this model is expected to exist in the data store.  This is the case for model instances loaded
     * from the data store as well as models constructed with a PK.  Saving changes for a materialized model will
     * result in an Update, while saving changes for a model that is not materialized will result in a Create.
     * 
     * @return  bool
     */
    public function isMaterialized()
    {
        return $this->materialized;
    }
    
    /**
     * Call the appropriate getter for the given property.  Note, to read generated auto-incremented PK values, call the
     * save method so the model is written to the data store and assigns the appropriate values before attempting to
     * read the value.
     * 
     * @param   string  $name
     * @return  mixed
     */
    public function __get( $name )
    {
        $methodName = 'get' . ucfirst( $name );
        return method_exists( $this, $methodName )
          ? $this->$methodName()
          : null;
    }
    
    /**
     * Call the appropriate setter for the given property.  Throw an exception if attempting to change a primary key
     * value on a materialized model.  If the property does not exist, silently fail.
     * 
     * @param   string  $name
     * @param   mixed   $value
     */
    public function __set( $name, $value )
    {
        $methodName = 'set' . ucfirst( $name );
        return method_exists( $this, $methodName )
          ? $this->$methodName( $value )
          : null;
    }
    
}