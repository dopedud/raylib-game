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
    static std::mutex locker;

    std::unique_ptr<EditorContext> context;

public:

    explicit Editor(PrivateKey);
    ~Editor();

    static Editor* instance();
    static void destroy();

    inline EditorContext* context() { return context.get(); }

    void draw();
};