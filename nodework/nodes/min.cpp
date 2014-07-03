// __NODE__   min
//
// __TYPES__  double QColor int QChar QString
//
// __INPUT__  a       Type
// __INPUT__  b       Type
// __OUTPUT__ min     Type
//
template<typename Type> Type min( const Type& a, const Type& b )
{
    return a <= b ? a : b;
}
