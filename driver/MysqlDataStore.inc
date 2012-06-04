<?php
/**
 * Copyright Skinit, Inc.
 */

class MysqlDataStore extends OrmDataStore implements ISchematic
{
    const BIG_INTEGER = 'bigint';
    const BINARY_LARGE_OBJECT = 'blob';
    const CHARACTER = 'char';
    const DATE = 'date';
    const DATE_TIME = 'datetime';
    const DECIMAL = 'decimal';
    const ENUMERATION = 'enum';
    const INTEGER = 'integer';
    const LONG_BINARY_LARGE_OBJECT = 'longblob';
    const LONG_TEXT = 'longtext';
    const MEDIUM_TEXT = 'mediumtext';
    const SET = 'set';
    const SMALL_INTEGER = 'smallint';
    const TEXT = 'text';
    const TIME = 'time';
    const TIMESTAMP = 'timestamp';
    const TINY_INTEGER = 'tinyint';
    const VARIABLE_CHARACTER = 'varchar';
    

    protected $connection = null;
    protected $connectionParams = array();

    /**
     * Set the connection parameters for the MySQL DB.
     * 
     * @param   string  $host
     * @param   string  $user
     * @param   string  $password
     * @param   string  $schema
     * @param   int     $port 
     */
    public function __construct( $host = 'localhost', $user = 'root', $password = null, $schema = 'information_schema',
            $port = 3306 )
    {
        $this->connectionParams = array(
            'host' => $host,
            'user' => $user,
            'password' => $password,
            'schema' => $schema,
            'port' => $port,
        );
    }
    
    /**
     * Connect to the MySQL server. 
     */
    public function connect()
    {
        // get the connection params for this data store
        $host = $this->connectionParams['host'];
        $user = $this->connectionParams['user'];
        $password = $this->connectionParams['password'];
        $schema = $this->connectionParams['schema'];
        $port = $this->connectionParams['port'];
        
        // connect to the MySQL server
        $this->connection = new mysqli( $host, $user, $password, $schema, $port );
    }

    public function getName()
    {
        return $this->connectionParams['schema'];
    }
    
    public function getRelations()
    {
        "select table_name from information_schema.tables";
    }

    public function getRelationAttributes( $relationName )
    {
        "select column_name from information_schema.columns where table_name = ?";
        "select data_type from information_schema.columns where table_name = ? and column_name = ?";

        switch( $dataType )
        {
            case self::BIG_INTEGER:
            case self::INTEGER:
            case self::SMALL_INTEGER:
            case self::TINY_INTEGER:
                return ISchematic::TYPE_INTEGER;
            case self::BINARY_LARGE_OBJECT:
            case self::CHARACTER:
            case self::ENUMERATION:
            case self::LONG_BINARY_LARGE_OBJECT;
            case self::LONG_TEXT:
            case self::MEDIUM_TEXT:
            case self::SET:
            case self::TEXT:
                return ISchematic::TYPE_TEXT;
            case self::DATE:
            case self::DATE_TIME:
            case self::TIME:
            case self::TIMESTAMP:
                return ISchematic::TYPE_DATE_TIME;
            case self::DECIMAL:
                return ISchematic::TYPE_NUMBER;
        }
    }
    
    public function getRelationships( $relationName )
    {
        "select referenced_table_name, column_name, referenced_column_name, ordinal_position";
        "from information_schema.key_column_usage where table_name = ?";
    }
    
}