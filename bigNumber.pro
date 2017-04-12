TEMPLATE = subdirs

SUBDIRS += src

CONFIG(debug, debug|release) {
    SUBDIRS += tests
}
CONFIG(release, debug|release): {
    DEFINES += NDEBUG
}
# Qt defines QT_NO_DEBUG for release builds. Otherwise QT_DEBUG is defined.
