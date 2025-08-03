#pragma once

#include <memory>

struct EditorContext
{

};

class Editor
{
private:

    struct PrivateKey { explicit PrivateKey() = default; };

    static std::unique_ptr<Editor> m_instance;
    static std::once_flag flag;

    std::unique_ptr<EditorContext> context;

public:

    explicit Editor(PrivateKey);
    ~Editor();

    static Editor* instance();

    inline EditorContext context() const { return context; }

    void draw();
};