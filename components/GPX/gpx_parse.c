#include "gpx_parse.h"
#include "stdio.h"

#define SD "/sdcard"

void start_element(void *data, const char *element_name, const char **attributes)
{
    // printf("Start element: %s\n", element_name);
    for (int i = 0; attributes[i]; i += 2)
    {
        printf("Attribute: %s = %s\n", attributes[i], attributes[i + 1]);
    }
}

void end_element(void *data, const char *element_name)
{
    // printf("End element: %s\n", element_name);
}

void character_data(void *data, const char *text, int text_length)
{
    // printf("Character data: %.*s\n", text_length, text);
}

int get_gpx_data()
{
    /* 05121439.gpx   05121258.gpx */
    FILE *file = fopen(SD "/05121439.gpx", "r");
    if (file == NULL)
    {
        printf("Failed to open XML file\n");
        return 1;
    }

    XML_Parser parser = XML_ParserCreate(NULL);
    if (parser == NULL)
    {
        printf("Failed to create XML parser\n");
        fclose(file);
        return 1;
    }

    XML_SetElementHandler(parser, start_element, end_element);
    XML_SetCharacterDataHandler(parser, character_data);

    char buffer[BUFSIZ];
    size_t bytesRead;

    while ((bytesRead = fread(buffer, 1, sizeof(buffer), file)) > 0)
    {
        if (XML_Parse(parser, buffer, bytesRead, bytesRead == 0) == XML_STATUS_ERROR)
        {
            printf("XML parsing error\n");
            break;
        }
    }

    XML_ParserFree(parser);
    fclose(file);
    return 1;
}