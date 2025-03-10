---@diagnostic disable: undefined-field
set_project("tcsl")
-- set_languages("c90")
set_languages("c99")
-- set_languages("c11")
-- set_languages("c17")
-- set_languages("c23")

rule("copy_headers")
    before_build(function ()
        os.cp("src/*.h", "$(buildir)/include/tcsl/")
    end)

target("tcsl")
    set_kind("static")
    add_files("src/*.c") 
    add_rules("copy_headers")

target("test_tcsl")
    set_kind("binary")
    add_files("tests/*.c")
    add_deps("tcsl")
    add_includedirs("$(buildir)/include/tcsl")



