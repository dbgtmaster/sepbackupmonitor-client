#include "html.h"

QString html::htmlspecialchars(QString str) {
    return str.replace("&", "&amp;").replace("\"", "&quot;").replace("'", "&#039;").replace("<", "&lt;").replace(">", "&gt;");
}
