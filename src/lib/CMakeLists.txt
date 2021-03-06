project(twablet)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
set(CMAKE_AUTOMOC TRUE)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5Core REQUIRED)
find_package(Qt5Network REQUIRED)
find_package(Qt5Qml REQUIRED)

add_definitions(-DQT_NO_CAST_FROM_ASCII)

include_directories(
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_BINARY_DIR}
    ${QT_INCLUDES}
)

set(${PROJECT_NAME}_Core_SRCS
    globals.h
    qobjectutils.h
    irepositorylistener.h
    iloadsave.h
    account.cpp
    layout.cpp
    tweet.cpp
    quotedtweet.cpp
    user.cpp
    entity.cpp
    mediaentity.cpp
    urlentity.cpp
    usermentionentity.cpp
    hashtagentity.cpp
    query.cpp
    loadsavemanager.cpp
    repository.h
    accountrepository.cpp
    layoutrepository.cpp
    tweetrepository.h
    userrepository.h
    tweetcentralrepository.cpp
    usercentralrepository.cpp
    iqueryexecutor.h
    ilistqueryhandler.cpp
    abstracttweetlistqueryhandler.cpp
    hometimelinequeryhandler.cpp
    mentionstimelinequeryhandler.cpp
    searchqueryhandler.cpp
    usertimelinequeryhandler.cpp
    favoritesqueryhandler.cpp
    abstractuserlistqueryhandler.cpp
    friendsqueryhandler.cpp
    followersqueryhandler.cpp
)

set(${PROJECT_NAME}_Private_SRCS
    private/twitterdatautil.cpp
    private/twitterqueryutil.cpp
    private/networkqueryexecutor.cpp
)

set(${PROJECT_NAME}_Qml_SRCS
    qml/accountobject.cpp
    qml/layoutobject.cpp
    qml/tweetobject.cpp
    qml/quotedtweetobject.cpp
    qml/userobject.cpp
    qml/mediaobject.cpp
    qml/queryobject.cpp
    qml/imodel.h
    qml/model.h
    qml/accountmodel.cpp
    qml/layoutmodel.cpp
    qml/tweetmodel.cpp
    qml/usermodel.cpp
    qml/mediamodel.cpp
    qml/querylistmodel.cpp
    qml/accountselectionmodel.cpp
    qml/twitterauthentification.cpp
    qml/datarepositoryobject.cpp
    qml/datarepositoryobjectmap.h
    qml/descriptionformatter.cpp
    qml/entitiesformatter.cpp
    qml/tweetformatter.cpp
    qml/quotedtweetformatter.cpp
    qml/abstractqueryitem.cpp
    qml/statusupdatequeryitem.cpp
    qml/tweetqueryitem.cpp
    qml/retweetqueryitem.cpp
    qml/favoritequeryitem.cpp
    qml/userqueryitem.cpp
)

add_library(${PROJECT_NAME} STATIC
    ${${PROJECT_NAME}_Core_SRCS}
    ${${PROJECT_NAME}_Private_SRCS}
    ${${PROJECT_NAME}_Qml_SRCS}
)

qt5_use_modules(${PROJECT_NAME} Core Network Qml)

set(${PROJECT_NAME}_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}
    CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE
)
