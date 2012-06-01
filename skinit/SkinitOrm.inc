<?php
/**
 * Copyright Skinit, Inc.
 */

class SkinitOrm
{
    const APP_WEBSITE = 'website';
    const APP_CATALOG = 'catalog';
    const APP_PAYMENT = 'payment';
    
    /**
     * Return the name of the current application.  This should be one of the SkinitOrm::APP_* constants.  Models for
     * applications that are different than the current one will act as remote models, while those that share the
     * current application will act as local models.
     *
     * @return  string
     */
    public static function getCurrentApplication()
    {

    }

}