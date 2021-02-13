import React, { useState } from 'react';
import {Alert} from 'react-native';
import { useSelector, useDispatch } from 'react-redux';
import Icon from 'react-native-vector-icons/MaterialIcons';
import {withNavigationFocus} from 'react-navigation';

import Background from '~/Components/Background';
import Input from '~/Components/Input';
import Button from '~/Components/Button';

import {Container, Title} from './styles';

import { updateControllerIp } from '~/store/modules/user/actions';

function Configurations() {  
  const dispatch = useDispatch();
  const socketIp = useSelector(state => state.user.controllerIP);
  const [ip, setIp] = useState('');

  function handleChange() {
    dispatch(updateControllerIp(ip));
    Alert.alert('Sucesso', 'Endereço alterado');
  }

  React.useEffect(() => {   
    if (socketIp) {
      setIp(socketIp);
    }    
  }, [socketIp]);

  return (
    <Background>
      <Container>
        <Title>Configurações</Title>
        <Input icon="network-wifi" value={ip} onChangeText={setIp} placeholder="Endereço do microcontrolador"/>
        <Button onPress={handleChange}>Salvar</Button>
      </Container>
    </Background>
  );
}

Configurations.navigationOptions = {
  tabBarLabel: 'Configurações',
  tabBarIcon: ({tintColor}) => (
    <Icon name="settings" size={20} color={tintColor} />
  ),
};

export default withNavigationFocus(Configurations);
