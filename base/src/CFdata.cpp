//  (C) Copyright 2005-2008 Benjamin Godon and contributors.
//
//  This file is part of Cellule.
//
//  Cellule is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  Cellule is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with Cellule; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//
#include "CFdata.h"
#include "CFenum.h"

#include <QtGui/QComboBox>
#include <QtGui/QCheckBox>
#include <QtGui/QSlider>

const CFtype CFtype::kInvalid( 0, "invalid" );

QMap<CFtype,CFdata> CFdata::m_defaultValues;
QMap<QString,int>   CFdata::m_typeNameToTypeId;

void CFdata::registerType( const CFtype& type, CFdata defaultValue )
{
    m_defaultValues.insert( type, defaultValue );

    m_typeNameToTypeId.insert( type.name(), type.id() );
}

void CFdata::registerBasicTypes( void )
{
    //  Registration associates a default value to each data type.
    //  This is needed for the CFdata constructor taking a CFtype to work.
    //
    CFdata::registerType( CFdata::type<bool           >(), false             );
    CFdata::registerType( CFdata::type<int            >(), 0                 );
    CFdata::registerType( CFdata::type<CFfloat        >(), CFfloat()         );
    CFdata::registerType( CFdata::type<CFenum         >(), CFenum()          );
    CFdata::registerType( CFdata::type<CFcolor        >(), CFcolor()         );
    CFdata::registerType( CFdata::type<CFcolorGradient>(), CFcolorGradient() );
}

CFtype CFdata::type( const QString& typeName )
{
    QMap<QString,int>::iterator typeIdIt( m_typeNameToTypeId.find( typeName ) );

    if( typeIdIt == m_typeNameToTypeId.end() ) return CFtype::kInvalid;

    return CFtype( typeIdIt.value(), typeName );
}

template <> CFtype CFdataAnyValue<bool           >::m_type = CFtype( 1, "boolean"       );
template <> CFtype CFdataAnyValue<int            >::m_type = CFtype( 2, "integer"       );
template <> CFtype CFdataAnyValue<CFfloat        >::m_type = CFtype( 3, "real"          );
template <> CFtype CFdataAnyValue<CFenum         >::m_type = CFtype( 4, "enum"          );
template <> CFtype CFdataAnyValue<CFcolor        >::m_type = CFtype( 5, "color"         );
template <> CFtype CFdataAnyValue<CFcolorGradient>::m_type = CFtype( 6, "colorGradient" );
