function [x_post, C_x_post] = lkf_meas_update_step(prob)

    % extract parameters and sensitivity matrices etc from model
    [h n para J] = get_model_settings(prob, prob.model.meas);
   
    % assuming the first argument is the old state (!)
    C_x_prior = para(1).uncert;
    x_prior = para(1).value;
 
    
    % get last made real measurement 
    [z] = prob.env.meas.result.value;
    
    %if no real measurement, then 
    if isnan(z(1,1))
        x_post = x_prior;
        C_x_post = C_x_prior;
        return
    end
    
    
        
    % make measurement prediction, based on sensitivity matrices
    %[z_hat] = feval(h, para(:).value);
    z_hat = (J(1).m)*para(1).value;
    
    for i=2:n,
        z_hat = z_hat + (J(i).m)*para(i).value;
    end;
    
    
    % calculate measurement residual
    [z_res] = z - z_hat;
    
    
    % calculate uncertainty in redidual
    % this is noise in prediction plus measurement
    for i=1:n,
        C_tmp(i).influence = J(i).m*para(i).uncert*(J(i).m');
    end;
        
    C_res = C_tmp(1).influence;
    if n>1, for i=2:n, C_res = C_res + C_tmp(i).influence; end; end;
    
    
    % ready for updates...
     
    % calculate Kalman gain   
    K = (C_x_prior*((J(1).m)'))*( inv(C_res) );
    
    
    % update state
    x_post = x_prior + K*z_res;
    
    % and uncertainty
    C_x_post = C_x_prior - K*(J(1).m)*C_x_prior;

    % done.
    pr 'lkf measurement update performed.';

