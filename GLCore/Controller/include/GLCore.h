#pragma once
#include <Controller.h>
#include <InputMapper.h>
#include <MainView.h>
#include <ModeManager.h>
#include <QObject>

class GLCore : public QObject {
    Q_OBJECT

    // parts
    MainView *main_view_{};
    InputMapper *input_mapper_{};
    ModeManager *mode_manager_{};
    Controller *controller_{};

    // shared vars
    ModeType mode_type_{};

    void initConnects() {
        // input (MainView -> InputMapper)

#define CONNECT_INPUT(CONNECT_TYPE) \
    connect(main_view_, &MainView::CONNECT_TYPE, \
            input_mapper_, &InputMapper::on##CONNECT_TYPE);

        CONNECT_INPUT(KeyPress);
        CONNECT_INPUT(KeyRelease);
        CONNECT_INPUT(MousePress);
        CONNECT_INPUT(MouseRelease);
        CONNECT_INPUT(MouseDoubleClick);
        CONNECT_INPUT(MouseMove);

#undef CONNECT_INPUT

        // command (InputMapper -> ModeManager)

        connect(input_mapper_, &InputMapper::command,
                mode_manager_, &ModeManager::onCommand);
    }

public:
    GLCore() {
        main_view_ = new MainView();
        controller_ = new Controller(main_view_->scene());

        input_mapper_ = new InputMapper(mode_type_);
        mode_manager_ = new ModeManager(controller_);

        initConnects();

        // add redo/undo actions
        main_view_->addActions(
            { mode_manager_->createUndoAction(main_view_), mode_manager_->createRedoAction(main_view_) }
        );
    }

    QGraphicsView *view() const {
        return main_view_;
    }

    ~GLCore() override {
        // use manual deleting because no reason to use qt parent system

        // from source to consumer
        delete main_view_;
        delete input_mapper_;
        delete mode_manager_;
        delete controller_;
    }
};