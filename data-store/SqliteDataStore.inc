<?php
/**
 * Copyright Skinit, Inc.
 */

class SqliteDataStore extends SqlDataStore
{
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
        return ISchematic::TYPE_TEXT;
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
        return $statement->execute();
    }
    
}