#include "VFSDirView.h"
#include "VFSFile.h"

VFS_NAMESPACE_START


DirView::DirView()
: DirBase("")
{
}

DirView::~DirView()
{
}

void DirView::init(const char *name)
{
    _setName(name);
    _view.clear();
}

void DirView::add(DirBase *dir)
{
    _view.push_back(dir);
}

File *DirView::getFileByName(const char *fn) const
{
    for(ViewList::const_reverse_iterator it = _view.rbegin(); it != _view.rend(); ++it)
        if(File *f = (*it)->getFileByName(fn))
            return f;
    return NULL;
}

void DirView::forEachDir(DirEnumCallback f, void *user, bool safe)
{
    for(ViewList::reverse_iterator it = _view.rbegin(); it != _view.rend(); ++it)
        (*it)->forEachDir(f, user, safe);
}

static void _addFileCallback(File *f, void *p)
{
    ((Files*)p)->insert(std::make_pair(f->name(), f)); // only inserts if not exist
}
void DirView::forEachFile(FileEnumCallback f, void *user, bool /*ignored*/)
{
    Files flist; // TODO: optimize allocation
    for(ViewList::reverse_iterator it = _view.rbegin(); it != _view.rend(); ++it)
        (*it)->forEachFile(_addFileCallback, &flist);

    for(Files::iterator it = flist.begin(); it != flist.end(); ++it)
        f(it->second, user);
}

bool DirView::_addToView(char *path, DirView& view)
{
    if(_view.empty())
        return false;

    for(ViewList::const_iterator it = _view.begin(); it != _view.end(); ++it) // not reverse!
        view.add(const_cast<DirBase*>(it->content()));
    return true;
}

static void __test__()
{
	new DirView;
}

VFS_NAMESPACE_END
