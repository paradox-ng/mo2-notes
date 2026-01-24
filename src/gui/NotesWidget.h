#pragma once

#include "qmarkdowntextedit.h"
#include <QFile>
#include <QPushButton>
#include <QStackedWidget>
#include <QToolBar>
#include <QVBoxLayout>
#include <QWebEngineView>

class NotesWidget final : public QWidget {
    Q_OBJECT

public:
    explicit NotesWidget(QWidget* parent = nullptr);
    ~NotesWidget() override;

    static void createDefaultMarkdownStyle(const QString& path);

    void setProfilePath(const QString& profilePath);

    void setDefaultToViewMode(bool viewMode);

    void reloadStyles() const;

    void saveNotes();

private slots:

    void onTextChanged();

    void toggleViewMode();

    void updatePreview() const;

    void setupMarkdownHighlighter() const;

    // Formatting slots
    void insertBold();
    void insertItalic();
    void insertStrikethrough();
    void insertHeading1();
    void insertHeading2();
    void insertHeading3();
    void insertLink();
    void insertImage();
    void insertInlineCode();
    void insertCodeBlock();
    void insertBulletList();
    void insertNumberedList();
    void insertCheckbox();
    void insertQuote();
    void insertHorizontalRule();

private:
    void initWebView() const;
    void initToolbar();
    void applyEditorStyles() const;
    void wrapSelection(const QString& before, const QString& after);
    void insertAtLineStart(const QString& prefix);

    QStackedWidget* m_stackedWidget;
    QMarkdownTextEdit* m_textEdit;
    QWebEngineView* m_webView;
    QVBoxLayout* m_layout;
    QToolBar* m_toolbar;
    QPushButton* m_toggleButton;
    QAction* m_toggleAction = nullptr;
    QString m_profilePath;
    QTimer* m_saveTimer;
    QTimer* m_previewTimer;
    bool m_isDirty    = false;
    bool m_isEditMode = true;
    int m_saveRetryCount = 0;
    static constexpr int MAX_SAVE_RETRIES = 3;
};
