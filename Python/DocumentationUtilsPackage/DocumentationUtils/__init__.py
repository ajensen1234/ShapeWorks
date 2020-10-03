from DocumentationUtils import CommandsUtils
from mdutils.mdutils import MdUtils


#%%

def generateShapeWorksCommandDocumentation(mdFilename = '../../docs/tools/ShapeWorksCommands.md', add_toc = False):
    
    # settings from Executable.cpp
    opt_width  = 32
    indent     = 2
    spacedelim = ''.ljust(indent)

    mdFile        = MdUtils(file_name = mdFilename, title = '')
    mdFile.new_header(level = 1, title = 'ShapeWorks Commands')
    
    # add intro paragraph
    intro_paragraph = "`shapeworks` is a single executable for ShapeWorks with a set of sub-executables (commands) that are flexible, modular, loosely coupled, and standardized subcommands, with interactive help to perform individual operations needed for a typical shape modeling workflow that includes the Groom, Optimize, and Analyze phases.\n"
    mdFile.new_paragraph(intro_paragraph)
    
    if add_toc:
        intro_marker = mdFile.create_marker(" ") # mark the after-intro to add table of contents after the introduction paragraph
    
    cmd           =  "shapeworks"    
    CommandsUtils.addCommand(mdFile, cmd, level = 2, spacedelim = spacedelim, verbose = True)
    
    if add_toc:
        mdFile.new_table_of_contents(table_title='Table of Contents', depth=3, marker = intro_marker)
    
    mdFile.create_md_file()
    
#%%

