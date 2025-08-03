#pragma once

struct EditorContext

class Editor
{
private:

    struct PrivateKey { explicit PrivateKey() = default; };

    static std::unique_ptr<Editor> m_instance;
    static std::once_flag flag;

public:

    explicit Editor(PrivateKey);
    ~Editor();

    static Editor* instance();

    void draw();
};