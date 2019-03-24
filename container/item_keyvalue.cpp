#include "item_keyvalue.h"


bool Item_KeyValue::setData(int k, QVariant v)
{
    switch (k) {
    case IK::key:key=v.toString();return true;
    case IK::value:value=v.toString();return true;
    case IK::isset:isset=v.toBool();return true;
    case IK::sort:sort=v.toInt();return true;
    case IK::isshow:isshow=v.toBool();return true;
    case IK::defvalue:defvalue=v.toString();return true;
    case IK::link:link=v.toStringList();return true;
    default:return false;
    }
}

bool Item_KeyValue::operator <(const Item_KeyValue &a) const
{
    return sort<a.sort;
}
