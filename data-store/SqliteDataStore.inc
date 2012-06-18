<?php
/**
 * Copyright Skinit, Inc.
 */

class SqliteDataStore extends SqlDataStore
{
    const INTEGER = 'integer';
    const TEXT = 'text';
    
    /** @var Sqlite3 $db */
    protected $db = null;
    protected $path = null;
    
    /**
     * Set the path to the Sqlite database.
     * 
     * @param   string  $path 
     */
    public function __construct( $path )
    {
        $this->path = (string)$path;
    }

    
    /**************************************************************************
     * SqlDataStore implementation 
     **************************************************************************/

    public function connect()
    {
        $this->db = new SQLite3( $this->path, SQLITE3_OPEN_READWRITE );
    }
    
    public function getName()
    {
        return basename( $this->path );
    }
    
    protected function mapDataType( $sqlType )
    {
        switch( $sqlType )
        {
            case self::INTEGER:
                return ISchematic::TYPE_INTEGER;
            default:
                return ISchematic::TYPE_TEXT;
        }
    }
    
    protected function query( $query, $values = array() )
    {
        // create a statement and bind params
        $statement = $this->db->prepare( $query );
        $i = 0;
        array_walk( $values, function($value) use($statement, $i) {
            $i++;
            $typeCode = is_int( $value ) ? SQLITE3_INTEGER : null;
            $typeCode = $typeCode ?: is_float( $value ) ? SQLITE3_FLOAT : SQLITE3_TEXT;
            $statement->bindParam( $i, $value, $typeCode );
        } );
        
        // execute the statement and return an iterator for the results
        $results = $statement->execute();
        return new SqliteResultsIterator( $results );
    }
    
    
    /**************************************************************************
     * ISchematic implementation 
     **************************************************************************/
        
    public function getRelations()
    {
        // retreive tables and build SchemaRelation instances describing them
        $query = "select name from sqlite_master where type = 'table'";
        $results = array();
        foreach( $this->queryColumn( $query ) as $tableName )
        {
            $results[] = new SchemaRelation( $this, $tableName );
        }
        
        // return the relations
        return $results;
    }

    public function getRelationAttributes( $relationName )
    {
        // grab the columns and types and build SchemaAttribute instances describing the columns
        $tableName = str_replace( '"', '""', $relationName );
        $query = "pragma table_info(\"$tableName\")";
        $results = array();
        foreach( $this->query( $query ) as $record )
        {
            $name = $record['name'];
            $dataType = $this->mapDataType( $record['type'] );
            $results[] = new SchemaAttribute( $this, $relationName, $name, $dataType );
        }

        // return the attributes
        return $results;
    }
    
    public function getRelationships( $relationName )
    {
        // grab FK constraints and build SchemaRelationship instances describing the FK constraints
        $query = "pragma foreign_key_list(?)";
        $values = array( $relationName );
        $results = array();
        $fromKey = array();
        $toKey = array();
        foreach( $this->query( $query, $values ) as $record )
        {
            // if we are at sequence number 0, we have a new relationship
            $sequence = $record['seq'];
            if( $sequence == 0 )
            {
                // before we move to the new relationship, generate the previous one
                if( $fromKey && $toKey )
                {
                    $fromKey = new SchemaKey( $fromKey );
                    $toKey = new SchemaKey( $toKey );
                    $results[] = new SchemaRelationship( $fromKey, $toKey );
                    $fromKey = array();
                    $toKey = array();
                }
                
                $attributeName = $record['from'];
                $matches = array_filter( $this->getRelationAttributes( $relationName ), function($attribute) use($attributeName) {
                    return $attribute->getName() == $attributeName;
                } );
                $fromKey[] = array_shift( $matches );
                
                $referencedRelation = $record['table'];
                $attributeName = $record['to'];
                $matches = array_filter( $this->getRelationAttributes( $referencedRelation ), function($attribute) use($attributeName) {
                    return $attribute->getName() == $attributeName;
                } );
                $toKey[] = array_shift( $matches );
            }
        }
        
        // create final relationship from leftover data
        if( $fromKey && $toKey )
        {
            $fromKey = new SchemaKey( $fromKey );
            $toKey = new SchemaKey( $toKey );
            $results[] = new SchemaRelationship( $fromKey, $toKey );
        }
                
        // return the relationship
        return $results;
    }    
    
}