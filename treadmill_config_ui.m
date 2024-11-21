classdef treadmill_config_ui < matlab.apps.AppBase

    % Properties that correspond to app components
    properties (Access = public)
        UIFigure            matlab.ui.Figure
        SessionLabel        matlab.ui.control.Label
        SessionField        matlab.ui.control.EditField
        AnimalCodeLabel     matlab.ui.control.Label
        AnimalCodeField     matlab.ui.control.EditField
        nTrialsLabel        matlab.ui.control.Label
        nTrialsField        matlab.ui.control.NumericEditField
        csDelayLabel        matlab.ui.control.Label
        csDelayField        matlab.ui.control.NumericEditField
        SaveButton          matlab.ui.control.Button
    end

    methods (Access = private)

        % Button pushed function: SaveButton
        function saveToWorkspace(app, ~)
            % Assign variables to base workspace
            assignin('base', 'session', app.SessionField.Value);
            assignin('base', 'animal_code', app.AnimalCodeField.Value);
            assignin('base', 'n_trials', app.nTrialsField.Value);
            assignin('base', 'cs_delay', app.csDelayField.Value);

            % Destroy the app
            delete(app.UIFigure);
        end
    end

    % Component initialization
    methods (Access = private)

        % Create UIFigure and components
        function createComponents(app)
            % Create UIFigure
            app.UIFigure = uifigure('Visible', 'off');
            app.UIFigure.Position = [100, 100, 400, 300];
            app.UIFigure.Name = 'Treadmill Config UI';

            % Create Session Label and Field
            app.SessionLabel = uilabel(app.UIFigure);
            app.SessionLabel.Position = [50, 250, 100, 22];
            app.SessionLabel.Text = 'Session:';
            app.SessionField = uieditfield(app.UIFigure, 'text');
            app.SessionField.Position = [160, 250, 180, 22];

            % Create Animal Code Label and Field
            app.AnimalCodeLabel = uilabel(app.UIFigure);
            app.AnimalCodeLabel.Position = [50, 200, 100, 22];
            app.AnimalCodeLabel.Text = 'Animal Code:';
            app.AnimalCodeField = uieditfield(app.UIFigure, 'text');
            app.AnimalCodeField.Position = [160, 200, 180, 22];

            % Create nTrials Label and Numeric Field
            app.nTrialsLabel = uilabel(app.UIFigure);
            app.nTrialsLabel.Position = [50, 150, 100, 22];
            app.nTrialsLabel.Text = 'nTrials:';
            app.nTrialsField = uieditfield(app.UIFigure, "numeric");
            app.nTrialsField.Position = [160, 150, 180, 22];

            % Create csDelay Label and Numeric Field
            app.csDelayLabel = uilabel(app.UIFigure);
            app.csDelayLabel.Position = [50, 100, 100, 22];
            app.csDelayLabel.Text = 'csDelay(s):';
            app.csDelayField = uieditfield(app.UIFigure, "numeric");
            app.csDelayField.Position = [160, 100, 180, 22];

            % Create Save Button
            app.SaveButton = uibutton(app.UIFigure, 'push');
            app.SaveButton.Position = [50, 30, 300, 40];
            app.SaveButton.Text = 'Run Configuration';
            app.SaveButton.ButtonPushedFcn = createCallbackFcn(app, @saveToWorkspace, true);

            % Show the figure after all components are created
            app.UIFigure.Visible = 'on';
        end
    end

    % App initialization and construction
    methods (Access = public)

        % Construct app
        function app = treadmill_config_ui
            % Create and configure components
            createComponents(app);

            % Pause execution until app is deleted
            uiwait(app.UIFigure);
        end
    end
end
