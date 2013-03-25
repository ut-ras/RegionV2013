#include <cstdio>
#include <libxml2/libxml/parser.h>
#include <libxml2/libxml/tree.h>

int main() {
    xmlDocPtr doc;
    doc = xmlReadFile("map.xml", NULL, 0);
    
    printf("%s",doc->name);
    
    // Free things
    xmlFreeDoc(doc);
    xmlCleanupParser();
    return 0; 
}
