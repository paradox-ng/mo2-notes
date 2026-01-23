#pragma once

#include "qmarkdowntextedit.h"
#include <QFile>
#include <QPushButton>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QWebEngineView>

class NotesWidget final : public QWidget {
    Q_OBJECT

public:
    explicit NotesWidget(QWidget* parent = nullptr);

    static void createDefaultMarkdownStyle(const QString& path);

    void setProfilePath(const QString& profilePath);

    void setDefaultToViewMode(bool viewMode);

    void reloadStyles() const;

private slots:
    void saveNotes();

    void onTextChanged();

    void toggleViewMode();

    void updatePreview() const;

    void setupMarkdownHighlighter() const;

private:
    void initWebView() const;

    void applyEditorStyles() const;

    QStackedWidget* m_stackedWidget;
    QMarkdownTextEdit* m_textEdit;
    QWebEngineView* m_webView;
    QVBoxLayout* m_layout;
    QPushButton* m_toggleButton;
    QString m_profilePath;
    QTimer* m_saveTimer;
    QTimer* m_previewTimer;
    bool m_isDirty    = false;
    bool m_isEditMode = true;
};
