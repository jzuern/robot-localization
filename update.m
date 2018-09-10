function [x_prior, C_x_prior] = lkf_time_update_step(prob)
% executes time update step of EKF, given a problem definition
 
    % extract parameters and Jacobians etc from model
    [f n para J] = get_model_settings(prob, prob.model.process);
    
 
    % update a-priori state, based on transfer matrices
    %x_prior = feval(f, para(:).value);
    
    x_prior = (J(1).m)*para(1).value;
    for(i=2:n),
       x_prior = x_prior + (J(i).m)*para(i).value;     
    end
    
    
    % calculate separate uncertainty propagations
    for i=1:n,
       C_tmp(i).influence = (J(i).m)*para(i).uncert*(J(i).m)';
    end
  
    % sum the separate uncertainties
    C_x_prior = C_tmp(1).influence;
    if n>1, for i=2:n, C_x_prior = C_x_prior + C_tmp(i).influence; end; end;
   
    % done.
  
    pr 'lkf time update performed.'

