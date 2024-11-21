function values = generateDACcosinewave(amplitude, frequency, baseline)
    
    % Check if all arguments are provided
    if nargin ~= 3
        error('All three arguments (amplitude, frequency, baseline) must be provided.');
    end
    
    % Time interval per point
    timeInterval = 0.0004985; % seconds
    
    % Calculate the period of the wave
    period = 1 / frequency; % seconds
    
    % Determine the number of points
    numPoints = round(period / timeInterval);
    
    % Calculate the time vector for one period
    t = linspace(0, 1, numPoints + 1);
    t = t(1:end-1);  % Remove the last point to ensure smooth period
    
    % Generate cosine wave values centered around the specified baseline
    values = amplitude * cos(2 * pi * t) + baseline;
end
