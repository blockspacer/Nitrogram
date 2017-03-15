TEMPLATE = subdirs

SUBDIRS += \
    IgApi \
    IgApiTest \
    IgApiPrimitive \
    Nitrogram

IgApi.subdir            = IgApi
IgApiTest.subdir        = IgApiTest
IgApiPrimitive.subdir   = IgApiPrimitive
Nitrogram.subdir        = Nitrogram

IgApiTest.depends       = IgApi
IgApiPrimitive.depndes  = IgApi
