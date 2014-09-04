#ifndef PANEHELPER_H
#define PANEHELPER_H

template<class T>
T findClosestParent(QWidget* widget)
{
    while(widget)
    {
        if (qobject_cast<T>(widget))
        {
            return static_cast<T>(widget);
        }
        widget = widget->parentWidget();
    }
    return 0;
}

#endif // PANEHELPER_H
