set(
    SOURCE "PageBase.cpp"
            "PM_Anim.cpp"
            "PM_Base.cpp"
            "PM_Drag.cpp"
            "PM_Router.cpp"
            "PM_Drag.cpp"
)

idf_component_register(SRCS ${SOURCE}
                    INCLUDE_DIRS .
                    REQUIRES lvgl )